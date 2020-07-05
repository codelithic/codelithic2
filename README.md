# Example codelithic
Example how to use codelithic_site

dependents: 
* log4cplus 
* codelithic 
* codelithic_site

install nginx with fcgi support

copy src/site/assetes/nginx/site.conf --> /etc/nginx/default.d 

nginx reload

Remember SELINUX!!
```
setsebool -P httpd_can_network_connect 1
```
https://stackoverflow.com/questions/23948527/13-permission-denied-while-connecting-to-upstreamnginx

change src/site/assetes/systemd/site.service with your path
```
cp src/site/assetes/systemd/site.service /etc/systemd/system/
```
Enable service:
```
systemctl enable site
```
start service:
```
systemctl start site
systemctl restart nginx
```
```
systemctl stop site
```

