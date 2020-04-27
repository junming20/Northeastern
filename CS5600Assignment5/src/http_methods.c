/*
 * methods.c
 *
 * Functions that implement HTTP methods, including
 * GET, HEAD, PUT, POST, and DELETE.
 *
 *  @since 2019-04-10
 *  @author: Philip Gust
 */

#include "http_methods.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <unistd.h>
#include <dirent.h>

#include "http_server.h"
#include "http_util.h"
#include "time_util.h"
#include "media_util.h"
#include "properties.h"
#include "string_util.h"
#include "file_util.h"


/**
 * Create a temp file with HTML of directory listing
 *
 * @param dirPath the directory path
 * @return the file stream to to directory listing
 */

static FILE *do_list_dir(const char *dir_path, const char *uri) {
	FILE *tmpstream = tmpfile();
	// If by chance temporary file can not be created,
	//then the tmpfile() function returns NULL pointer
	if (tmpstream == NULL) {
		return NULL;
	}

	//output header
	fprintf(tmpstream,
		"<html>"
	    "<head><title>index of %s</title></head>"
	    "<body>"
	    "<h1>Index of %s</h1>\n"
		"<table>\n<tr>\n"
		"<th>Name</th><th>Last modified</th><th>Size</th><th>Description</th>"
		"\n</tr>\n<tr><td colspan=\"5\"><hr></td></tr>\n",
		uri, uri
	);

	DIR *dir = opendir(dir_path);
	struct dirent entry, *result;
	//readdir_r() function returns 0 on success
	while ((readdir_r(dir, &entry, &result) == 0) && (result != NULL)) {
		//exclude current directory, adn parent directory for root
		if (  (strcmp(entry.d_name, ".") == 0)
		    || ((strcmp(uri, "/") == 0) && (strcmp(entry.d_name, "..") == 0))) {
			continue;
		}

		//make parent direct entry as "Parent Directory"
		const char *name;
		if (strcmp(entry.d_name,"..") == 0) {
			name = "Parent Directory";
		} else {
			name = entry.d_name;
		}

		//combine path and filename for this entry
		char fname[PATH_MAX];
		makeFilePath(dir_path, entry.d_name, fname);


		//get properties for this entry
		struct stat sb;
		stat(fname, &sb);

		//get latest modification time for this entry
		char mtime[MAXBUF];
		milliTimeToShortHM_Date_Time(sb.st_mtim.tv_sec, mtime);

		//to form the url by appending "/"
		char url[MAXBUF];
		if (S_ISDIR(sb.st_mode)) {
			sprintf(url, "%s/", entry.d_name);
		} else {
			sprintf(url, "%s", entry.d_name);
		}


		// output directory entry
		fprintf(tmpstream,
			"<tr>\n"
			"<td><a href=\"%s\">%s</a></td>"
			"<td align=\"right\">%s</td><td align=\"right\">%lu</td><td>%s</td>\n"
			"</tr>\n",
			url, name, mtime, (unsigned long)sb.st_size, ""
		);
	}
	closedir(dir);

	// output footer
	fprintf(tmpstream,
		"<tr><td colspan=\"5\"><hr></td></tr>\n"
		"</body></html>"
	);

	fflush(tmpstream);
	rewind(tmpstream);
	return tmpstream;

}



/**
 * Handle GET or HEAD request.
 *
 * @param stream the socket stream
 * @param uri the request URI
 * @param requestHeaders the request headers
 * @param responseHeaders the response headers
 * @param sendContent send content (GET)
 */
static void do_get_or_head(FILE *stream, const char *uri, Properties *requestHeaders, Properties *responseHeaders, bool sendContent) {
	// get path to URI in file system
	char filePath[MAXPATHLEN];
	resolveUri(uri, filePath);
	FILE *contentStream = NULL;

	// ensure file exists
	struct stat sb;
	if (stat(filePath, &sb) != 0) {
		sendErrorResponse(stream, 404, "Not Found", responseHeaders);
		return;
	}
	// ensure directory path ends with '/'
	if (S_ISDIR(sb.st_mode) && (filePath[strlen(filePath)-1] == '/')) {
		contentStream = do_list_dir(filePath, uri);
		if (contentStream == NULL) {
			sendErrorResponse(stream, 405, "Method Not Allowed", responseHeaders);
			return;
		}
		fileStat(contentStream, &sb);
	} else if (!S_ISREG(sb.st_mode)) { // error if not regular file
		sendErrorResponse(stream, 404, "Not Found", responseHeaders);
		return;
	}

	// record the file length
	char buf[MAXBUF];
	size_t contentLen = (size_t)sb.st_size;
	sprintf(buf,"%lu", contentLen);
	putProperty(responseHeaders,"Content-Length", buf);

	// record the last-modified date/time
	time_t timer = sb.st_mtim.tv_sec;
	putProperty(responseHeaders,"Last-Modified",
				milliTimeToRFC_1123_Date_Time(timer, buf));

	// get mime type of file
	getMediaType(filePath, buf);
	if (strcmp(buf, "text/directory") == 0) {
		// some browsers interpret text/directory as a VCF file
		strcpy(buf,"text/html");
	}
	putProperty(responseHeaders, "Content-type", buf);

	// send response
	sendResponseStatus(stream, 200, "OK");

	// Send response headers
	sendResponseHeaders(stream, responseHeaders);


	if (sendContent) {  // for GET
		if (contentStream == NULL) {
			contentStream = fopen(filePath, "r");
		}
		copyFileStreamBytes(contentStream, stream, contentLen);
	}
	if (contentStream != NULL) {
		fclose(contentStream);
	}
}

/**
 * Handle GET request.
 *
 * @param stream the socket stream
 * @param uri the request URI
 * @param requestHeaders the request headers
 * @param responseHeaders the response headers
 * @param headOnly only perform head operation
 */
void do_get(FILE *stream, const char *uri, Properties *requestHeaders, Properties *responseHeaders) {
	do_get_or_head(stream, uri, requestHeaders, responseHeaders, true);
}

/**
 * Handle HEAD request.
 *
 * @param the socket stream
 * @param uri the request URI
 * @param requestHeaders the request headers
 * @param responseHeaders the response headers
 */
void do_head(FILE *stream, const char *uri, Properties *requestHeaders, Properties *responseHeaders) {
	do_get_or_head(stream, uri, requestHeaders, responseHeaders, false);
}

/**
 * Handle DELETE request.
 *
 * @param the socket stream
 * @param uri the request URI
 * @param requestHeaders the request headers
 * @param responseHeaders the response headers
 */

void do_delete(FILE *stream, const char *uri, Properties *requestHeaders, Properties *responseHeaders) {
    // get path to URI in file system
    char filePath[MAXPATHLEN];
    resolveUri(uri, filePath);

    // ensure file exists
    struct stat sb;
    if (stat(filePath, &sb) != 0) {
        sendErrorResponse(stream, 404, "Not Found", responseHeaders);
        return;
    }

    // if a directory
    if (S_ISDIR(sb.st_mode) && strendswith(filePath, "/") ) {
        if (rmdir(filePath) == 0) {
            sendResponseStatus(stream, 200, "Ok");
            sendResponseHeaders(stream, responseHeaders);
        } else {
            sendErrorResponse(stream, 405, "Method Not Allowed", responseHeaders);
        }
        //if a regular file
    } else if (S_ISREG(sb.st_mode)) {
        if (unlink(filePath) == 0) {
            sendResponseStatus(stream, 200, "Ok");
            sendResponseHeaders(stream, responseHeaders);
        } else {
            sendErrorResponse(stream, 405, "Method Not Allowed", responseHeaders);
        }
    } else {
        sendErrorResponse(stream, 404, "Not Found", responseHeaders);
    }
}

/**
 * Handle PUT request.
 *
 * @param the socket stream
 * @param uri the request URI
 * @param requestHeaders the request headers
 * @param responseHeaders the response headers
 */
void do_put(FILE *stream, const char *uri, Properties *requestHeaders, Properties *responseHeaders) {
    // get path to URI in file system
    char filePath[MAXPATHLEN];
    resolveUri(uri, filePath);
    FILE *putStream = NULL;

    //check if file exist already, before any operation
    struct stat stats;
    bool existed = false;
    if(stat(filePath, &stats != 0)){
        existed = false;
    }
    else{
        existed = true;
    }

    // create intermediate directories
    char path[MAXPATHLEN];
    if (getPath(filePath, path) != NULL) {
        mkdirs(path, 0755);
    }

    putStream = fopen(filePath, "w");
    if (putStream == NULL) {
        // not allowed for this method
        sendErrorResponse(stream, 405, "Method Not Allowed", responseHeaders);
        return;
    }
    char buf[MAXBUF];
    // return the index of the value found or SIZE_MAX if not found
    if (findProperty(requestHeaders, 0, "Content-Length", buf) == SIZE_MAX) {
        sendErrorResponse(stream, 411, "Length Required", responseHeaders);
    } else {
        copyFileStreamBytes(stream, putStream, atoi(buf));
    }
    putProperty(responseHeaders, "Content-Length", "0");

    if(existed){
        sendResponseStatus(stream, 200, "Ok");
        sendResponseHeaders(stream, responseHeaders);
    } else {
        sendResponseStatus(stream, 201, "Created");
        sendResponseHeaders(stream, responseHeaders);
    }
    fclose(putStream);
}


/**
 * Handle POST request.
 *
 * @param stream the socket stream
 * @param uri the request URI
 * @param requestHeaders the request headers
 * @param responseHeaders the response headers
 * @param sendContent send content (GET)
 */
void do_post(FILE *stream, const char *uri, Properties *requestHeaders, Properties *responseHeaders) {
    // get path to URI in file system
    char filePath[MAXPATHLEN];
    resolveUri(uri, filePath);
    FILE *postStream = NULL;

    // create intermediate directories
    char path[MAXPATHLEN];
    if (getPath(filePath, path) != NULL) {
        mkdirs(path, 0755);
    }

    postStream = fopen(filePath, "w");
    if (postStream == NULL) {
        // not allowed for this method
        sendErrorResponse(stream, 405, "Method Not Allowed", responseHeaders);
        return;
    }

    char buf[MAXBUF];
    // return the index of the value found or SIZE_MAX if not found
    if (findProperty(requestHeaders, 0, "Content-Length", buf) == SIZE_MAX) {
        sendErrorResponse(stream, 411, "Length Required", responseHeaders);
    } else {
        copyFileStreamBytes(stream, postStream, atoi(buf));
    }
    putProperty(responseHeaders, "Content-Length", "0");

    sendResponseStatus(stream, 200, "Ok");
    sendResponseHeaders(stream, responseHeaders);

    fclose(postStream);
}
