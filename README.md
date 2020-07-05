# Exanple codelithic
Example how to use codelithic_site

install nginx with fcgi support

copy src/site/assetes/nginx/site.conf --> /etc/nginx/default.d 

nginx reload

Remember SELINUX!!
setsebool -P httpd_can_network_connect 1
https://stackoverflow.com/questions/23948527/13-permission-denied-while-connecting-to-upstreamnginx

src/site/assetes/systemd/site.service
cp src/site/assetes/systemd/site.service /etc/systemd/system/
```
systemctl enable site
systemctl start site
systemctl stop site
```

