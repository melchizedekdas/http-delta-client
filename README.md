# http-delta-client
client to implement delta response to improve caching

Step 1: Pre-processing at server at the time of resource modification
Every time there is a change in any resource at the server, the server will compare the latest version of the resource with some of its earlier versions and for each of the earlier versions, it finds the parts of the resource which have changed in the current version. It will check if the ratio of new data added in the new version to total size of the resource is below a pre-configured threshold, then it will proceed to create a ".parts" file of the resource for the two version combinations.
Format of .change file:
File name - <resource name>_<old version id>.parts
Content:
[<byte start1>-<byte end1>](<size of content1>)<content1>
[<byte start2>-<byte end2>](<size of content2>)<content2>
.
.

[<byte start1> - <byte end1>] is the byte number in the old version of the resource from which byte to which byte (both exclusive) have to be replaced with the new content {<content1>} of the new version to make it same as the latest version.

The server fill this information for every instance of change in between the two versions.

Step 2: HTTP GET Request by client
The client asks for the resource to the server with a few important headers:
1)version-id
2)accept-parts="yes"

By putting the version id it tells the server the version it has cached in its local memory. accept-part="yes" tells the server that the client is capable of understanding parts file.

Step 3: HTTP response by the server
The server looks at the version-id to check if it is the same or not.
if yes, the normal cache re-use routine follows.
If no, it looks for the accept-parts="yes" header. If it is not present, it sends the whole resource in the response in the traditional fashion.
But if it is present it checks if it has the corresponding .parts file for the resource and the version id in the request.
If it gets it gets the .parts file, it sends the HTTP response in the following format:
status=200,
version-id,
parts="yes"

The version id is the new version id. parts="yes" tells the client that the response body contains the .part file and should be processed differently.

Response body is the content of the .parts file created in step 1.

Step 4: At client after getting parts response
Read the resource from local upto <byte start1> then append <content1> to it then resume reading from <byte end1> till <byte start2> and repeat the process till the response body and local resource file are completely read. Once this process is over, the client has the content of the new version of the resource it requested.
