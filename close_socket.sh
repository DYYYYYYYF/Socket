#! /bin/sh
 
kill -9 $(lsof -i:10800| cut -c 9-15)
