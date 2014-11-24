create table x (sno int, primary key(sno));
insert into x values(1);
insert into x values(2);
insert into x values(3);
insert into x values(4);
insert into x values(5);
insert into x values(6);
insert into x values(7);
insert into x values(8);

create table student (
    sno char(8),
    sname char(16) unique,
    primary key ( sno ),
    sage int,
    sgender char (1)
    
);


insert into student values ('12345678','w',22,'M');
insert into student values ('12355678','a',22,'M');
insert into student values ('12545678','b',22,'M');
insert into student values ('15345678','c',22,'M');
insert into student values ('52345678','d',22,'M');
insert into student values ('12348678','e',22,'M');
insert into student values ('12345078','z',22,'M');
insert into student values ('12222678','f',22,'M');

insert into student values ('12345666','g',22,'M');
insert into student values ('312010xxxx', 'koalas', 20, 'F');
insert into student values ('3120101984','jt',21,'M');
insert into student values ('3120102066','xkt',21,'M');




--create index stunameidx on student ( sname  );
--drop index stunameidx;

--select * from student;




--drop table student;


select * from student where sno = '88888888';
select * from student where sage >= 20 and sgender = 'F';



delete from student;
exit;
delete from student where sno = '88888888';

create table family (
		title char(8),
		name char(16) unique,
		age int,
		gender char (1),
		weight float,
		primary key ( title )
);

insert into family values ('daddy', 'gold', 24, 'M', 55.5);
insert into family values ('mommy', 'kola', 23, 'F', 49.5);
insert into family values ('baby', 'Baby Kola Gold', 5, 'F', 29.5);
insert into family values ('mommy', 'wife2', 23, 'F', 60);



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
