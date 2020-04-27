# assignment-5
team members: Junming Zhao(id:junming20), Hongming Zhang(id:runpirate), Aravind Somasundaram (aravindsomu) Mian Huang (mxh810) Xintong Liu (liuxinto01)

DUE DATE: Tue, April 21 2020 by 11:59:59pm
An HTTP Server
The HTTP server provided with this exercise is sometimes known as a "tiny http server" because it supports the most basic web server functionaity. In this asignment, you will have an opportunity to make several significant enhancements to this HTTP server and to learn more about networking, threads, synchronization, and this kind of stateless client-server architecture.

Begin with the source code we studied in Lecture 12, which is located in the 2020SPCS5600SV/assignment-5 Github repository. The repository contains a "src" directory with the source code for the tiny http server, a "content" directory that contains HTML files and subdirectories representing the server's content root directory, and several example and configuration files. Create a project in your IDE with the code in the "src" directory, and put the other files and directory wherever you have configured your server directory in your http configuration file.

Adding DELETE, PUT, POST HTTP methods
This initial implemenetation supports two of the five main methods that an HTTP server can perform. One of them is GET. This method requests the server to return a document from its hierarchically-organized file and directory system. The other method is HEAD. This command returns the same information about the document as GET, but HEAD does not return the response body.

You will implement the other three HTTP methods, which are used by web applications to delete and store documents in the web server's file system. These operations are DELETE, PUT, and POST.

The DELETE HTTP method has the same format as GET. The named document is deleted from the server if it exists and the server returns a 200 "OK" response. If the file is not found, it returns a "404 "Not Found" response. For the purposes of this exercise, allow deleting a directory, but only if it is empty. The path must have a trailing path seperator to indicate that is is a directory. If the directory is not empty, return 405 "Method not Allowed." Use CURL to issue a DELETE request:
curl -X "DELETE" http://localhost:8080/myfile.html
Implement it in the C function do_delete(File *stream, cconst char *uri, Properties *requestProperties, and Properties *responseHeaders) that is called by process_request().
The PUT HTTP method stores a document on the HTTP server at the location specified by the URL. If the file is created in the server, the server returns a 201 "Created" response. If the named file is overwritten, the server returns a 200 "OK" response. The content is sent as the request body, after the empty line that ends the request parameters. If the file already exists, its content is replaced. If the server output file cannot be cannot opened, the server returns 405: "Method Not Allowed". If the content length is not specified in the request header, the server should send back 411: "Length Required". For the purposes of this exercise, create any intermediate directories specified by the request path. Use CURL to issue a PUT request:
curl http://localhost:8080/serverfile.html --upload-file localfile.html
Implement it in the C method do_put(FILE *stream, const char* uri, Properties *requestHeaders, Properties *responseHeaders) that is called by process_request().
The POST HTTP method is used to request that the server accept the content as a new subordinate of the resource identified by the requiest URL. Essentially, the URL is a collection URL. For example it is used to submit form data to a back-end form processor like an ordering system. POST sends just the form data as the request body. The server returns "200 "OK" in response. If the server output file cannot be cannot opened, the server returns 405: "Method Not Allowed". If the content length is not specified in the request header, the server should send back 411: "Length Required". For the purposes of this exercise, create any intermediate directories specified by the request path.
The HTML form encoding type application/x-www-form-urlencoded sends the field names and values in the request body as a single line, with the name separated from the value with an equal sign ("="), and name/value pairs separated by an ampersand ("&"). Values are URL-encoded (spaces are converted to "+" symbols, and special characters are converted to ASCII HEX values) in case they contiain any special characters like equal sign or amersand. The content type is application/x-www-form-urlencoded. For example, the fields values
Name: Gareth Wylie
Age: 24
Formula: a + b == 13%!
are encoded as
Name=Gareth+Wylie&Age=24&Formula=a+%2B+b+%3D%3D+13%25%21
Use CURL to issue a POST request that URL-encodes its data values like an HTML form (on one line):
curl --data-urlencode 'Name=Gareth Wylie' --data-urlencode 'Age=24' --data-urlencode 'Formula=a + b == 13%!' http://localhost:8080/formfile.txt

The "form-post.html" file in the "forms" subdirectory of the "content" directory that uses the default application/x-www-form-urlencoded encoding type.
The HTML form encoding type multipart/form-data sends the field names and values in the request body as a multi-part form with each field name and value in its own part, separated from other parts by special delimiter lines. This type of form is used to send non-text valued field values, such as documents, images, and other non-text data. The parts include length information that enable the fields and their values to be extracted and processed by a form-processing back end. This is how an image of a user would be sent as part of a registration form for a website.

Use CURL to issue a POST request that includes a "Profile-pic" field whose value is a PNG file of the user:
curl --form "Name=Gareth Wylie" --form "Age=24" --form "Profile-pic=@/Users/gareth/gareth.png"

The "form-post-multipart.html" file in the "forms" subdirectory of the "content" directory that uses the multipart/form-data encoding type and allows a profile image to be uploaded.
Implement the POST processing code in the C method do_post(FILE *stream, const char* uri, Properties *requestHeaders, Properties *responseHeaders) that is called by process_request(). For the purposes of this assignment, the method simply stores the request body in a file and returns

Listing directories
Add code to list the contents of a directory as a formatted HTML page. This method is an extension of GET and the server returns a 200 "OK" response. The content is sent as the body of the response, after the empty line that ends the request parameters. The path must have a trailing path seperator to indicate that is is directory. If the directory does not exist, return a "404 "Not Found" response. Use CURL to issue a GET request, in this case to list the server's root content directory:

curl http://localhost:8080/
Add code to do_get() that determines whether the specified file is a directory. If so, it generates the directory listing and returns it as the body of the response. A recommendation is to write a function that generates the directory listing as a temporary file and returns a FILE*, and then have do_get() return the contents of that file.

For each entry, the directory listing should include the name of the entry, the last modified date of the form "2019-04-13 19:03", the size of the file or directory, and optionally any descriptive information. The entries should include a link to the content or for a file, or to the subdirectory if a directory. Subdirectories should include a "Parent Directory" entry to return to the previous level.

Here are sample listings for a root directory and a subdirectory that correspond closely with the default output for the Apache web server.

Improved media type processing
The code in "media_types.c" detects the type of a file and returns the media type. A media type (formerly known as MIME type) is a two-part identifier for file formats and format contents transmitted on the Internet. The current implementaiton uses a small set of hard-wired tests. Production HTTP servers use more flexible mechanisms. One of the earliest was to use a file named "mime.types" that has lines with media types and zero or more associated file extensions. The HTTP server reads this file at startup. A copy of a typical "mime.types" file is included with your assignment. Replace the hard-wired mechanism with one that uses the information in this file.

A good implementation strategy is to read the file extensions and media types into a Properties instance when starting the server, then looking up the media type using the file extension as the key. Add a readMediaTypes() method to media_util.c that accepts the name of the "mime.types" file and builds a static global Properties instance with file extensions as keys and media types as values. If there is no Properties instance, this method creates one. If there is a Properties instance, this method replaces it with one created from this file. The method returns the number of entries in Properties. Reimplement the getMediaType() method to use this Properties instance to get the media type. If the file extension is not registered, return the default media type.

Add a new "ContentTypes" property to the HTTP configuration file that specifies the "mime.types" file. A relative path is relative to the server root directory. Add code to the process_config() method in http_server.c that reads the "ContentTypes" property and calls readMediaTypes() with the file name to initialize the media types. If the property is not defined in the configuration file, use "mime.types" as the default value. If no entries are read, process_config() should return false.

More advanced HTTP servers use a variety of techniques including the file extension, as well as examining or "sniffing" the format of the document. An example of a framework for detecting media type using a variety of techniques is Apache Tika library. You may want to explore this library to see how it works.

Threading
This server only processes a single request at a time. A limited number of other requests are queued by the socket layer. Your next task will be to add threading to the HTTP server. For each client request, you will create a separate thread to handle that request. The main() function calls process_request() with the client socket to process the request. This is the function that you will need to thread.

Rather than creating raw threads, you will instead use a framework called a thread pool to queue tasks waiting for a thread from a fixed pool to become available. When one does, the thread pool selects a task waiting in the queue and assigns the free threed to run the task. When the task completes, the thread is reurned to the pool, ready to run another task

For this exercise use a C thread pool implementation located in the 2020SPCS5600SV/assignment-5-threadpool Github repository. This is a modified version of the original written by Johan Hanssen Seferidis, which is available at https://github.com/Pithikos/C-Thread-Pool. This modified version has been updated to support CygWin. The repository contains .c and a .h files that implement the thread pool, and an example program to show you how to use it. You may either add these files to your project, or put the code in its own project and use your skills at creating libraries to create a library that you can link to from your own project. You should choose a reasonable number of threads to test out your threaded code request processor.

Project Staffing
You can work in teams of 1-4 people on this assignment. The README.md file must show the name and the CCIS ID of each team member. As with past assignments, please turn in all the files from the original assignment, even though you have not modified some of them. This will make it easier for the graders to run your code.

Turning in the assignment
Turn your assignment in by uploading all the .c and .h files to your 2020SPCS5600SV/assignment-5-ccis_id repository. Do not turn in other files that may have been created by your IDE. The graders will download and run your code, so be sure that all the files are avaiable and no extraneous files have been checked in. Once you have checked in your code, it is a good idea to verify that what you checked in is actually in the repository to avoid delays in grading your assignment.

You are welcome to check in intermediate versions during development as a backup to what is on your computer. Only the final version will be graded.

Important: If you are working as part of a team, both of you should check in your work in your respective repositories. You both must also edit your "README.md" files to list the names and the CCIS-IDs of both of you.
