#ifndef BEEBLEBROX_PERSISTENCE_QUERY_ROUTE_HPP
#define BEEBLEBROX_PERSISTENCE_QUERY_ROUTE_HPP

#define QUERY_LOGIN ("select id from users where name=$1 and password=$2")
#define QUERY_SERVER_CONFIG ("select * from beeblebrox")

#endif