[![PGXN version](https://badge.fury.io/pg/hashtypes-aleksabl.svg)](https://badge.fury.io/pg/hashtypes-aleksabl)
[![Build Status](https://travis-ci.org/aleksabl/hashtypes.svg?branch=master)](https://travis-ci.org/aleksabl/hashtypes)
# About this project
This project is a fork of https://github.com/infofarmer/hashtypes. This fork provides the following enhancements:

* Support for BINARY export/import
* Support for aggregate functions (min/max)
* Fixed versioning with regards to PGXN
* Support for parallelism (for Postgresql 9.6+)
* Some performance enhancements


# Installation
The easiest way to get started with this project is to use PGXN.

First: Install pgxn (`sudo apt install pgxnclient` or similar for other distros). Then run
`sudo pgxnclient install aleksabl-hashtypes`.

# Use

Connect to the database you want. First you need to install the extension for the current database: `CREATE EXTENSION hashtypes`.

Example usage:
```
CREATE TABLE shatest(a sha256)
INSERT INTO shatest VALUES ('aec750369ad69194f076f675afae3872e8d1a5f3b6c68ec3f9278f');
INSERT INTO shatest VALUES ('000000369ad69194f076f675afae3872e8d1a5f3b6c68ec3f9278f');

select * from shatest where a > '100000000000000000000000000000000000000000000000000000');
```
