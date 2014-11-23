create table student (
    sno char(8),
    sname char(16) unique,
    primary key ( sno ),
    sage int,
    sgender char (1)
    
);

--create index stunameidx on student ( sname  );
--drop index stunameidx;
insert into student values ('12345678','w',22,'M');
insert into student values ('12355678','a',22,'M');
insert into student values ('12545678','b',22,'M');
insert into student values ('15345678','c',22,'M');
insert into student values ('52345678','d',22,'M');
insert into student values ('12348678','e',22,'M');
insert into student values ('12345678','f',22,'M');
insert into student values ('12345666','g',22,'M');


--select * from student;




--drop table student;


select * from student where sno = '88888888';
select * from student where sage >= 20 and sgender < 'F';



delete from student;
exit;
delete from student where sno = '88888888';

create table student (
		sno char(8),
		sname char(16) unique,
		sage int,
		sgender char (1),
		score float,
		primary key ( sno )
);

delete from student;
insert into student values ('12345678',	'wy1',22,'M',95);
insert into student values ('12345679',	'wy2',19,'F',100);
create index stunameidx on student ( sname );
insert into student values ('12345682',	'wy5',14,'M',60);
insert into student values ('12345684',	'wy6',25,'F',63);
select * from student;
select * from student where sno = '12345679';
select * from student where score >= 90.0 and score <=95;
select * from student where score > 60 and score <65;
select * from student where score >= 98;
select * from student where sage > 20 and sgender = 'F';
delete from student where sno = '12345678';
delete from student where sname = 'wy2';
select * from student;
insert into student values ('12345681',	'wy4',23,'F',96);
insert into student values ('12345670',	'wy3',25,'M',0);
select * from student where score < 10;
select * from student where sgender <> 'F';
drop index stunameidx;
drop table student;
