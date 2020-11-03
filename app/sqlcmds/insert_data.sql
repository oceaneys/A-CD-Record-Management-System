-- SQL for inserting data to the three tables

-- delete exsiting data firstly

delete from cd;
delete from artist;
delete from track;

-- Insert data to artist;

insert into artist(id, name) values(1, "Pink Floyd");
insert into artist(id, name) values(2, "Genesis");
insert into artist(id, name) values(3, "Einaudi");
insert into artist(id, name) values(4, "Melanie C");


-- Insert data to cd table;

insert into cd(id, title, artist_id) values(1, "Dark side of the moon",1);
insert into cd(id, title, artist_id) values(2, "Wish you were Here",1);
insert into cd(id, title, artist_id) values(3, 'A Trick of the Tail', 2);
insert into cd(id, title, artist_id) values(4, 'Selling England By the Pound', 2);
insert into cd(id, title, artist_id) values(5, 'I Giorni', 3);                                                                       
insert into cd(id, title, artist_id) values(6, 'Northern Star', 4);


-- Insert data to track table;

insert into track(cd_id, track_id, title) values(1, 1, 'Speak to me');
insert into track(cd_id, track_id, title) values(1, 2, 'Breathe');
insert into track(cd_id, track_id, title) values(1, 3, 'On the run');
insert into track(cd_id, track_id, title) values(1, 4, 'Time');
insert into track(cd_id, track_id, title) values(1, 5, 'Great gig in the sky');
insert into track(cd_id, track_id, title) values(1, 6, 'Money');
insert into track(cd_id, track_id, title) values(1, 7, 'Us and them');
insert into track(cd_id, track_id, title) values(1, 8, 'Any colour you like');
insert into track(cd_id, track_id, title) values(1, 9, 'Brain damage');
insert into track(cd_id, track_id, title) values(1, 10, 'Eclipse ');

insert into track(cd_id, track_id, title) values(2, 1, 'Shine on you crazy diamond');
insert into track(cd_id, track_id, title) values(2, 2, 'Welcome to the machine');
insert into track(cd_id, track_id, title) values(2, 3, 'Have a cigar');
insert into track(cd_id, track_id, title) values(2, 4, 'Wish you were here');
insert into track(cd_id, track_id, title) values(2, 5, 'Shine on you crazy diamond pt.2');

insert into track(cd_id, track_id, title) values(3, 1, 'Dance on a volcano');
insert into track(cd_id, track_id, title) values(3, 2, 'Entangled');
insert into track(cd_id, track_id, title) values(3, 3, 'Squonk');
insert into track(cd_id, track_id, title) values(3, 4, 'Madman moon');
insert into track(cd_id, track_id, title) values(3, 5, 'Robbery assault and battery');
insert into track(cd_id, track_id, title) values(3, 6, 'Ripples');
insert into track(cd_id, track_id, title) values(3, 7, 'Trick of the tail');
insert into track(cd_id, track_id, title) values(3, 8, 'Los Endos');

insert into track(cd_id, track_id, title) values(4, 1, 'Melodia Africana (part 1)');
insert into track(cd_id, track_id, title) values(4, 2, 'I due fiumi');
insert into track(cd_id, track_id, title) values(4, 3, 'In un\'altra vita');
insert into track(cd_id, track_id, title) values(4, 4, 'Melodia Africana (part 2)');
insert into track(cd_id, track_id, title) values(4, 5, 'Stella del mattino');
insert into track(cd_id, track_id, title) values(4, 6, 'I giorni');
insert into track(cd_id, track_id, title) values(4, 7, 'Samba');
insert into track(cd_id, track_id, title) values(4, 8, 'Melodia Africana (part 3)');
insert into track(cd_id, track_id, title) values(4, 9, 'La nascita delle cose segrete');
insert into track(cd_id, track_id, title) values(4, 10, 'Quel che resta');
insert into track(cd_id, track_id, title) values(4, 11, 'Inizio');
insert into track(cd_id, track_id, title) values(4, 12, 'Limbo');
insert into track(cd_id, track_id, title) values(4, 13, 'Bella notte');
insert into track(cd_id, track_id, title) values(4, 14, 'Canzone Africana (part 4)');

insert into track(cd_id, track_id, title) values(6, 1, 'Go!');
insert into track(cd_id, track_id, title) values(6, 2, 'Northern Star');
insert into track(cd_id, track_id, title) values(6, 3, 'Goin\' Down');
insert into track(cd_id, track_id, title) values(6, 4, 'I Turn To You');
insert into track(cd_id, track_id, title) values(6, 5, 'If That Were Me');
insert into track(cd_id, track_id, title) values(6, 6, 'Never Be The Same Again');
insert into track(cd_id, track_id, title) values(6, 7, 'Why');
insert into track(cd_id, track_id, title) values(6, 8, 'Suddenly Monday');
insert into track(cd_id, track_id, title) values(6, 9, 'Ga Ga');
insert into track(cd_id, track_id, title) values(6, 10, 'Be The One');
insert into track(cd_id, track_id, title) values(6, 11, 'Closer');
insert into track(cd_id, track_id, title) values(6, 12, 'Feel The Sun');
