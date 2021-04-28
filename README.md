This projects builds a HTTP caching proxy server which can handle GET, POST, CONNECT requests.
* We added concurrency to handle requests from different endpoints. 
* We used TCP sockets to send and receive packets.
* We cached responses according to validation and expiration rules defined in RFC7234.

This project is deployed with docker, to run this app, cd into the docker-deploy folder. Then run 
```
sudo docker-compose up 
```
Now you need to change the proxy of your web browser to this proxy.
After that you can open web pages with this proxy.  
The testcases we used is in ```testcases.txt``` and ```danger_log.txt``` contains some information about our program.  
