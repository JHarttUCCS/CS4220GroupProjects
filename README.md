# CS4220GroupProjects
CS4220 Group Projects

## Project 1
Group Members: Jake Hartt, Darin Rawson, Lea Karsanbhai

We/I have neither given nor received unauthorized assistance on this work.

### Program Description
This program was developed to help us better understand how to apply security concepts in networking, especially with protocols like Transport Layer Security (TLS) and Secure Sockets Layer (SSL). This can help us learn more about how we can protect information that is sent over a network. The information between a web server and client must be both encrypted and authenticated to help protect user information, and creating this network allows us to see how this is done. We can see potential risks for security, especially since private information is often sent over the internet, so it is important to ensure that we are keeping that data safe when it is transmitted. The scenario we are working with in this project is the idea of having a web browser (client) accessing a website (server) that they must share private information with, such as a credit card number, so we must allow for a secure transmission. We want this information to be secure and not be able to be read by a malicious actor, so we must implement secure practices to protect the information.


### How to Run the Program
1. Navigate to `CS4220GroupProjects/Project1`
2. Run `make all`
3. Run the server with `../out/httpserver`
4. Open another terminal on the same machine.
5. Navigate to `CS4220GroupProjects/Project1`
6. Run the client with `../out/httpclient`
7. You should see the returned text from http:
> Hello from the HTTPS server!


### Pieces of the Assignment (with a brief description)
##### Keys
These are the TLS certificate and key generated for use in our project.  Since this is a school project, there is no harm in uploading the authentication information here.

##### Makefile
The makefile builds the code and outputs it to the project's `out` folder.

##### Server
The server sets up a TLS+HTTP connection and listens for a secure TLS connection.  When one is recieved, it sends a simple message and then closes.

##### Client
The server establishe a TLS+HTTP connection with the server and recieves its message before printing it out and terminating.

##### Legacy Code
The `old` and `openssl_experiment` folders contain our initial code as we were figuring out how to program http and TLS respectively.  This code was vital in our final product, and was much of it was repurposed in the final code.


### Challenges
1. When designing the TLS client, we originally miswrote which buffer we were writing to, and got the buffers confused.  This was another way of learning how context-sensitive everything around our operations is.
2. When building the Makefile, we noticed that we could not build the OpenSSL code with a default set up.   In the end, we had to add custom SSL flags to the compiler to get it to compile SSL code, just like you would with something like _GNU_SOURCE.
3. While creating the HTTP server, an initial problem encountered was being unfamiliar with network programming and the various predefined functions that could be used. I was not sure where to start and was unfamiliar with the various components of an HTTP server. However, I was able to solve this by researching the HTTP protocol before I started writing any code. I initially learned about Transmission Control Protocol (TCP) and User Datagram Protocol (UDP) as well as sockets. The main resources I used to get this initial knowledge were *Hands-on network programming with C* and *Beej's Guide to Network Programming: Using Internet Sockets*. Through these resources, I learned that TCP would be used to create the HTTP server because it is a connection-oriented protocol, so it provides things like ensuring that data arrives in the same order that it was sent in, avoiding identical data from arriving more than once, and resending missing data. I also learned about the various socket functions that are often used in socket programming that would help create the HTTP server. With this knowledge, I started to look more at the structure of a general TCP program, and then began applying this to create an HTTP server.
4. One of the challenges faced while creating the HTTP server was with error handling. Since the server only handles GET requests, there is an error check to ensure that the client’s request is a GET request. However, there was not proper error checking to handle this, so the program would run even when the client was not using a GET request. To fix this, we had to add an exit statement if the request by the client was not a GET request.
5. Implementing HMAC was surprisingly simple.  Originally, we attempted to implement a bespoke HMAC solution by manually transmitting an HMAC tag on top of the default hash in the ciphersuite.  However, it turned out all we needed to do was specify a specific ciphersuite that uses HMAC to achieve HMAC implementation.  https://ciphersuite.info/cs/TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA384/


### Notes
[This project](https://github.com/darrenjs/openssl_examples) inspired much of the TLS code in the project.

Additionally, to learn more about network programming to create the initial HTTP server code, the main resources used were *Hands-on network programming with C* and *Beej's Guide to Network Programming: Using Internet Sockets*. These helped with getting an understanding of the primary functions that would be used in the server code as well as the general layout an HTTP server must have. *Beej's Guide to Network Programming: Using Internet Sockets* helped to get a better understanding of sockets and TCP while *Hands-on network programming with C* helped with learning the functions that should be used to create an HTTP server.


#### List of Resources Used (APA):

Baeldung. (2024, June 23). Creating a Self-Signed Certificate With OpenSSL. https://www.baeldung.com/openssl-self-signed-cert 

Darrenjs. (n.d.). Darrenjs/openssl_examples: Examples of using openssl. GitHub. https://github.com/darrenjs/openssl_examples 

Fielding, R., Gettys, J., Mogul, J., Frystyk, H., Masinter, L., Leach, P., & Berners-Lee, T. (1999, June). Hypertext transfer protocol -- HTTP/1.1. RFC Editor. https://www.rfc-editor.org/rfc/rfc2616 

Hall, B. (n.d.). Beej’s Guide to Network Programming. https://beej.us/guide/bgnet/html/split/index.html 

Khlebnikov, A., & Adolfsen, J. (2022). Demystifying Cryptography with OpenSSL 3.0. Packt Publishing, Limited. 

Linux manual pages: Alphabetic list of all pages. (n.d.). https://man7.org/linux/man-pages/dir_all_alphabetic.html 

OpenSSL Documentation. (n.d.). https://docs.openssl.org/3.2/ 

Rescorla, E. (2000, May). HTTP over TLS. RFC Editor. https://www.rfc-editor.org/rfc/rfc2818 

Rudolph, H. C., & Grundmann, N. (n.d.). Weak TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA384. Ciphersuite Info. https://ciphersuite.info/cs/TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA384/ 

Tanenbaum, A. S., & Wetherall, D. (2011). Computer Networks. Pearson Prentice Hall. 

Van Winkle, L. (2019). Hands-on network programming with C. Packt Publishing.
