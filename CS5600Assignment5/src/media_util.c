/*
 * media_util.c
 *
 * Functions for processing media types.
 *
 *  @since 2019-04-10
 *  @author: Philip Gust
 */

#include "media_util.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "string_util.h"
#include "http_server.h"
#include "file_util.h"
#include "properties.h"

/** default media type */
static const char *DEFAULT_MEDIA_TYPE = "application/octet-stream";

static Properties *media_property = NULL;


/**
 * Helper method to load media type properties from given file.
 *
 * @paam propFile the properties file
 * @param props the properties
 * @return number of properties read
 */
int loadMediaTypeProperties(const char *propFile, Properties *props) {
    FILE *propStream = fopen(propFile, "r");
    if (propStream == NULL) {
        return 0;
    }
    char buf[MAXBUF];
    int nprops = 0;

    // get next line
    while (fgets(buf, MAXBUF, propStream) != NULL) {
        if (buf[0] == '#') { // ignore comment
            continue;
        }

        // value is the content type that server is returning, which appears first in each line of the media type file
        char *value = strtok(buf, " \t\n");

        if (value != NULL) {
            // file extensions will be the keys to find corresponding value
            char *key;
            while ((key = strtok(NULL, " \t\n")) != NULL) {
                // if key is not null
                if (key[0] != '\0') {
                    putProperty(props, key, value);
                    nprops++;
                }
            }
        }
    }

    fclose(propStream);
    return nprops;
}

/**
* Read media type file and build a static global Property of media types.
*
* @param filename the name of the file
* @return number of entries in Properties
*/
int readMediaTypes(const char *filename) {
    //delete the existing media type property and replacing it with the new one below
    if (media_property != NULL) {
        deleteProperties(media_property);
    }

    //create a media type property
    media_property = newProperties();

    //call helper function to load the media type properties
    return loadMediaTypeProperties(filename, media_property);
}

/**
 * Return a media type for a given filename.
 *
 * @param filename the name of the file
 * @param mediaType output buffer for mime type
 * @return pointer to media type string
 */
char *getMediaType(const char *filename, char *mediaType)
{
	// special-case directory based on trailing '/'
	if (filename[strlen(filename)-1] == '/') {
		strcpy(mediaType, "text/directory");
		return mediaType;
	}

	// get file extension
    char ext[MAXBUF];
    if (getExtension(filename, ext) == NULL) {
    	// default if no extension
    	strcpy(mediaType, DEFAULT_MEDIA_TYPE);
    	return mediaType;
    }

    // lower-case extension
    strlower(ext, ext);

    // use ext as key to find the value from media_property and store it into mediaType for return
    size_t result = findProperty(media_property, 0, ext, mediaType);

    // if there's no media type property or we can't find the key/value pair, use default media type
    if((result == SIZE_MAX) || (media_property == NULL)){
        strcpy(mediaType, DEFAULT_MEDIA_TYPE);
    }

    return mediaType;
}
