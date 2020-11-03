-- SQL for creating tables



-- Drop all the exsit tables firstly

drop table cd;
drop table artist;
drop table track;


-- recreate

create table cd (
	id INTEGER AUTO_INCREMENT NOT NULL PRIMARY KEY,
	title VARCHAR(100) NOT NULL,
	artist_id INTEGER NOT NULL
);

create table artist (
	id INTEGER AUTO_INCREMENT NOT NULL PRIMARY KEY,
	name VARCHAR(100) NOT NULL
);

create table track (
	cd_id INTEGER NOT NULL,
	track_id INTEGER NOT NULL,
	title VARCHAR(100),
	PRIMARY KEY(cd_id, track_id)
);

