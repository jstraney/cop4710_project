BEGIN;

USE `event`;

-- reset auto-increments
ALTER TABLE `events` AUTO_INCREMENT = 1;
ALTER TABLE users AUTO_INCREMENT = 1;
ALTER TABLE universities AUTO_INCREMENT = 1;
ALTER TABLE commented_on AUTO_INCREMENT = 1;
ALTER TABLE events AUTO_INCREMENT = 1;
ALTER TABLE rsos AUTO_INCREMENT = 1;

-- this is an sql script which will insert test records
-- whenever we drop and re-upload our database.
-- It would be cool to write an 'install script' with the option of test records
-- but for now, you can just copy this query and paste it into phpMyadmin under the
-- SQL tab of the proper table.
INSERT INTO universities (name, location, lat, lon, email_domain, website_url, population)
VALUES
("University of Central Florida", "4000 Central Florida Blvd, Orlando, FL 32816", 28.602427, -81.200060, "@knights.ucf.edu", "http://www.ucf.edu", 64318),
("Florida State University", "600 W College Ave, Tallahassee, FL 32306", 30.441878, -84.298489, "@noles.fsu.edu", "http://www.fsu.edu", 41867),
("University of Kansas", "1450 Jayhawk Blvd, Lawrence, KS 66045", 38.954344, -95.255796, "@kansas.ppl.edu", "http://www.kans.edu", 28401),
("University of Michigan", "500 S State St, Ann Arbor, MI 48109", 42.278044, -83.738224, "@mich.ppl.edu", "http://www.mich.edu", 44718),
("Cornell University", "Ithaca, NY 14850", 42.453449 , -76.473503, "@corn.ppl.edu", "http://www.corn.edu", 21904),
("Yale University", "New Haven, CT 06520", 41.316324, -72.922343, "@yale.ppl.edu", "http://www.yale.edu", 12385);

-- all users have a password of 1234
CALL create_user (
  "jstraney", -- username
  "jeff", -- firstname
  "straney", -- last_name
  "jeff@gmail.com", -- e-mail
  'SA', -- role
  "$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", -- password. All passwords are 1234
   0 -- university id. Super Admin has no university
);
-- create all other users.
CALL create_user ("jbilabog","joe", "bilabog", "bilabog@knights.ucf.edu", 'STU', "$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq",  1);
CALL create_user("rdoorstopper", "robert", "doorstopper", "doorstopper@knights.ucf.edu", 'STU', "$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", 1);  
CALL create_user("rfaust", "raymond", "faust", "faust@knights.ucf.edu", 'STU',"$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", 1);
CALL create_user("rolson","ross", "olson",  "rolson@knights.ucf.edu", 'STU',"$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", 1);
CALL create_user("rafaelthepainter", "rafael", "caprese", "rafael@knights.ucf.edu", 'STU',"$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", 1);
CALL create_user("primrose", "rachel", "primrose", "rprimrose@knights.ucf.edu", 'STU',"$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", 1);
CALL create_user("rsandcastle", "robert", "sandcastle", "sandcastle@yale.ppl.edu", 'STU',"$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", 6);
CALL create_user("jdoorstopper", "josh", "doorstopper", "doorstopper@noles.fsu.edu", 'STU',"$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", 2);
CALL create_user("jonasborg", "jonas", "borg", "jborg@kansas.ppl.edu", 'STU',"$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", 3);
CALL create_user("jbosch", "jenifer", "bosch", "jbosch@mich.ppl.edu", 'STU',"$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", 4);
CALL create_user("ttarmach", "travis", "tarmach", "travis@mich.ppl.edu", 'ADM',"$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", 4);

-- ucf admin
CALL create_user("smackley", "sara", "mackley", "sara@knights.ucf.edu", 'ADM',"$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", 1);

-- michigan administrator
CALL create_user('jsmarshall', 'jacob', 'marshall', 'jmarshall@mich.ppl.edu', 'STU', '$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq', 4);

-- some michigan students
CALL create_user("jmarch", "james", "march", "jmarch@mich.ppl.edu", 'STU',"$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", 4);
CALL create_user("jtally", "jake", "tally", "jtally@mich.ppl.edu", 'STU',"$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", 4);
CALL create_user("jporche", "joshua", "porche", "jporche@mich.ppl.edu", 'STU',"$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", 4);
CALL create_user("jbailey", "jacob", "bailey", "jbailey@mich.ppl.edu", 'STU',"$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", 4);

-- some kansas students
CALL create_user("mreilly", "marc", "reilly", "mreilly@kansas.ppl.edu", 'STU',"$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", 3);
CALL create_user("mtosh", "maxine", "tosh", "mtosh@kansas.ppl.edu", 'STU',"$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", 3);
CALL create_user("mpoland", "marco", "poland", "mpoland@kansas.ppl.edu", 'STU',"$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", 3);
CALL create_user("mslater", "marchall", "slater", "mslater@kansas.ppl.edu", 'STU',"$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", 3);
CALL create_user("mtravertine", "michelle", "travertine", "mtravertine@kansas.ppl.edu", 'STU',"$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", 3);

-- some users at Yale
CALL create_user("fchris", "freddy", "chris", "fchris@yale.ppl.edu", 'STU',"$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", 6);
CALL create_user("folson", "fabio", "olson", "folson@yale.ppl.edu", 'STU',"$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", 6);
CALL create_user("frembrant", "falcon", "rembrant", "frembrant@yale.ppl.edu", 'STU',"$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", 6);
CALL create_user("fleonardo", "faust", "leonardo", "fleonardo@yale.ppl.edu", 'STU',"$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", 6);
CALL create_user("fflemming", "fehey", "flemming", "fflemming@yale.ppl.edu", 'STU',"$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", 6);
-- admin
CALL create_user("flozenges", "frank", "lozenges", "flozenges@yale.ppl.edu", 'ADM',"$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", 6);

-- FSU students
CALL create_user("palton", "pierre", "alton", "palton@noles.fsu.edu", 'STU',"$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", 2);
CALL create_user("pmappleleaf", "perry", "mappleleaf", "pmappleleaf@noles.fsu.edu", 'STU',"$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", 2);
CALL create_user("pramtha", "picard", "ramtha", "pramtha@noles.fsu.edu", 'STU',"$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", 2);
CALL create_user("psolomon", "penny", "solomon", "psolomon@noles.fsu.edu", 'STU',"$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", 2);
CALL create_user("pmarine", "pete", "marine", "pmarine@noles.fsu.edu", 'STU',"$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", 2);
-- admin
CALL create_user("pplymouth", "perry", "plymouth", "pplymouth@noles.fsu.edu", 'ADM',"$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", 2);

CALL create_rso(
  'STU', -- role of user creating RSO. In this case, jbillabog
  2, -- id of user creating rso
  'Surprise Society', -- name
  -- description
  'So, our thing is to hide in innocuous places and startle passerby\'s. This may seem like a harmless passtime, but is highly effective at unravling the fabric of civilization as we know it. We meet every third Wednesday of the month... when you\'d least expect it.',
  '2,3,4,5,6', -- members
  2, -- admin, jbillabog
  1 -- university, UCF
);

CALL create_rso(
  'ADM', -- role of user creating RSO. In this case, smackley 
  13, -- id of user creating rso
  'Debate Club', -- name
  -- description
  'Do you like to fight people? That might be a problem. But theres good news! Now there\'s a club where you can cultivate your contrarian attitudes. At debate club, you\'ll learn how to assert your views and make friends at the same time. Travel abroad, enter contests, practice public speaking. Club dues are $5.00 and should be paid upon submitting a request to join',
  '2,13,5,6,4', -- members
  13, -- admin, smackley 
  1 -- university, UCF
);

CALL create_rso(
  'STU', -- role of user creating RSO. In this case, jbosch 
  '11', -- id of user creating rso.
  'Michigan Kids Unite', -- name
  -- description
  'Are you in Michigan, and a kid? Look no further! Our club curates the coolest things to do. This club is free to join and plans trips twice a year. No one will be turned away! You are all invited. We love you.',
  '11,12,14,15,16', -- user id's of members.
   11, -- admin,  jbosch
   4 -- university, Univ. of Michigan 
);

CALL create_rso(
  'ADM', -- role of user creating RSO. In this case, ttarmach. This overrides the 5 member requirement
  '12', -- id of user creating rso. ttarmach, an administrator
  'Mich Doctors club', -- name
  -- description
  'Earning your doctorates? Passionate about your field? We are too. Let\'s talk about what fascinates us and make some breakthroughs. Non-doctorate program students are welcome to sit in and participate',
  '11,12', -- members. since the rso is being made by an administrator, the 5 member condition is waived. 
   11, -- admin,  jbosch
   4 -- university, Univ. of Michigan 
);

CALL create_rso(
  'STU', -- role of user creating RSO. In this case, ttarmach. This overrides the 5 member requirement
  '31', -- id of user creating rso. ttarmach, an administrator
  'FSU RSO', -- name
  -- description
  'Tootsie roll jelly beans cheesecake cheesecake pie marshmallow sesame snaps ice cream. Muffin cake dessert I love I love sweet roll liquorice ice cream sugar plum. Marshmallow ice cream bear claw bear claw wafer tart lollipop jelly brownie. Marshmallow apple pie jelly beans. Candy canes dragée ice cream sesame snaps dessert jelly beans dessert apple pie liquorice. Danish cheesecake donut gingerbread. Cupcake jelly-o biscuit. Pastry dragée I love candy soufflé gummi bears tootsie roll I love chocolate cake.',
  '31,32,33,34,35', -- members. 
   31, -- admin  
   2 -- university, FSU 
);

-- example of created event
CALL create_event(
  'Pizza Party', -- name
  '2017-07-11 11:00:00', -- start time
  '2017-07-11 13:00:00', -- end time
  '123-456-7890', -- telephone
  'billabog@knights.ucf.edu', -- email
  'Lets get pizza and play pong. Bring your favorite paddle and your appetite. No entry costs.', -- description
  '1151 University Blvd, Orlando, FL 32817, USA', -- location
  '28.598338', -- lat
  '-81.219712', -- lon
  'PUB', -- accessibility
  '2', -- user id, jbillabog
  '0', -- rso id. no rso. should be pending status as a result
  '1'-- university id
);

CALL create_event(
  'First Debate', -- name
  '2017-07-11 11:00:00', -- start time - time overlaps with pizza party but is in different location
  '2017-07-11 13:00:00', -- end time
  '123-456-7890', -- telephone
  'billabog@knights.ucf.edu', -- email
  'Lets have our first debate. This will be on the topic of foreign policy. How should the United States handle its external affairs, ya know?', -- description
  'Health & Public Affairs II, Orlando, FL 32816, USA', -- location
  '28.603014', -- lat
  '-81.198650', -- lon
  'PUB', -- accessibility
  '2', -- user id, jbillabog
  '2', -- rso id. debate club at UCF 
  '1'-- university id, UCF
);

CALL create_event(
  'Second Debate', -- name
  '2017-07-18 11:00:00', -- start time - time overlaps with pizza party but is in different location
  '2017-07-18 13:00:00', -- end time
  '123-456-7890', -- telephone
  'billabog@knights.ucf.edu', -- email
  'Lets have our second debate. This will be on the topic of ethics in science. Would it be a bad idea to clone humans, ya know?', -- description
  'Health & Public Affairs II, Orlando, FL 32816, USA', -- location
  '28.603014', -- lat
  '-81.198650', -- lon
  'PUB', -- accessibility
  '2', -- user id, jbillabog
  '2', -- rso id. debate club at UCF 
  '1'-- university id, UCF
);

CALL create_event(
  'Surprise Stakeout', -- name
  '2017-07-11 12:00:00', -- start time
  '2017-07-11 14:00:00', -- end time
  '123-456-7890', -- telephone
  'sara@knights.ucf.edu', -- email
  'Today, we will be hiding by the shrubs of the student union. Bring your maximum stealth.',
  'Apollo Cir, Orlando, FL 32816, USA', -- location
  '28.605335', -- lat
  '-81.198346', -- lon
  'RSO', -- accessibility
  '13', -- user id, smackley 
  '1', -- rso id. no rso. Surprise Society. Will be active as a result 
  '1'-- university id
);

CALL create_event(
  'First Doctor Meeting', -- name
  '2017-07-11 11:00:00', -- start time -- this overlaps an earlier event, but is in a different location.
  '2017-07-11 13:00:00', -- end time
  '123-456-7890', -- telephone
  'ttarmach@mich.ppl.edu', -- email
  'Let\'s meet at the chipotle\'s near campus. Bring your club dues! We will discuss this years plans and how we can best prepare!', -- description
  '235 S State St, Ann Arbor, MI 48104, USA', -- location
  '42.279316', -- lat
  '-83.740442', -- lon
  'PUB', -- accessibility. since it's public. it will be seen by members of other schools.
  '12', -- user id, ttarmach 
  '3', -- rso id. no rso. should be pending status as a result
  '4'-- university id, University of Michigan
);

CALL create_event(
  'Second Doctors Meeting', -- name
  '2017-07-18 11:00:00', -- start time 
  '2017-07-18 13:00:00', -- end time
  '123-456-7890', -- telephone
  'ttarmach@mich.ppl.edu', -- email
  'So we meet again!!! Bring a notebook!', -- description
  '235 S State St, Ann Arbor, MI 48104, USA', -- location
  '42.279316', -- lat
  '-83.740442', -- lon
  'PRI', -- accessibility. should only be visible to students at michigan university
  '12', -- user id, ttarmach 
  '3', -- rso id. no rso. should be pending status as a result
  '4'-- university id, University of Michigan
);

CALL create_event(
  'FSU Event 1', -- name
  '2017-07-12 15:00:00', -- start time -- this overlaps an earlier event, but is in a different location.
  '2017-07-12 18:00:00', -- end time
  '123-456-7890', -- telephone
  'pmappleleaf@noles.flsu.edu', -- email
  -- description written in cupcake ipsum
  'Sweet roll cupcake marzipan oat cake macaroon bear claw halvah soufflé macaroon. Gummi bears I love gingerbread. Biscuit I love I love cupcake topping. Icing macaroon bear claw caramels sesame snaps. Tart chupa chups gummi bears sesame snaps ice cream jujubes topping pastry cupcake. Sweet roll chocolate icing candy apple pie bonbon topping. Cotton candy topping ice cream.',
  '918 Way, Tallahassee, FL 32306, USA', -- location
  '30.441878', -- lat
  '-84.298489', -- lon
  'PUB', -- accessibility. since it's public. it will be seen by members of other schools.
  31, -- user id, some fsu student 
  5, -- rso id. 
  2-- university id, FSU 
);

CALL create_event(
  'FSU Event 2', -- name
  '2017-07-12 15:00:00', -- start time -- this overlaps an earlier event, but is in a different location.
  '2017-07-12 18:00:00', -- end time
  '123-456-7890', -- telephone
  'pmappleleaf@noles.flsu.edu', -- email
  -- description written in cupcake ipsum
  'I love I love soufflé toffee pie I love soufflé. Cake danish brownie. Gummies jelly beans I love. Lemon drops carrot cake candy marzipan. Ice cream lemon drops chocolate bar jelly beans lemon drops jujubes. Dessert jelly caramels soufflé cookie sugar plum chocolate cake chocolate bar. Wafer pastry chocolate cake I love pastry brownie.',
  '3300 Capital Cir SW, Tallahassee, FL 32310, USA', -- location
  '30.395407', -- lat
  '-84.345052', -- lon
  'PRI', -- accessibility. since it's public. it will be seen by members of other schools.
  31, -- user id, some FSU student 
  5, -- rso id. 
  2-- university id, FSU 
);

CALL create_event(
  'FSU Event 2', -- name
  '2017-07-19 15:00:00', -- start time -- this overlaps an earlier event, but is in a different location.
  '2017-07-19 18:00:00', -- end time
  '123-456-7890', -- telephone
  'pmappleleaf@noles.flsu.edu', -- email
  -- description written in cupcake ipsum
  'Gummi bears I love carrot cake chocolate bar jujubes toffee jelly beans sesame snaps tart. Fruitcake fruitcake marshmallow. Soufflé jujubes wafer macaroon. Jelly beans ice cream caramels. Candy jujubes candy canes sugar plum jelly liquorice halvah I love. Donut bonbon pudding tootsie roll soufflé. Muffin chupa chups I love.',
  'Woodward Ave. Garage, Tallahassee, FL 32304, USA', -- location
  '30.444573', -- lat
  '-84.298888', -- lon
  'PUB', -- accessibility. since it's public. it will be seen by members of other schools.
  31, -- user id, some FSU student 
  0, -- rso id.  no rso
  2-- university id, FSU 
);

CALL create_event(
  'FSU Event 3', -- name
  '2017-07-19 19:00:00', -- start time -- this overlaps an earlier event, but is in a different location.
  '2017-07-19 20:00:00', -- end time
  '123-456-7890', -- telephone
  'pmappleleaf@noles.flsu.edu', -- email
  -- description written in cupcake ipsum
  'Chocolate pudding macaroon tart caramels I love bonbon I love tiramisu. Brownie candy canes lemon drops I love pastry cake tiramisu jelly beans gummies. Bear claw cupcake fruitcake gummi bears. Powder biscuit jelly pastry gummies I love chocolate cake caramels caramels. Liquorice oat cake lemon drops I love soufflé pie donut I love. I love biscuit marzipan caramels I love. Topping wafer marzipan toffee carrot cake.',
  '3945 Museum Rd, Tallahassee, FL 32310, USA', -- location
  '30.410471', -- lat
  '-84.344054', -- lon
  'RSO', -- accessibility. since it's public. it will be seen by members of other schools.
  31, -- user id, some FSU student 
  5, -- rso id. 
  2-- university id, FSU 
);

CALL create_event(
  'FSU Event 4', -- name
  '2017-07-20 15:00:00', -- start time -- this overlaps an earlier event, but is in a different location.
  '2017-07-20 18:00:00', -- end time
  '123-456-7890', -- telephone
  'pmappleleaf@noles.flsu.edu', -- email
  -- description written in cupcake ipsum
  'Sweet pie ice cream brownie. Jujubes brownie macaroon jelly beans brownie. Bonbon macaroon tiramisu I love dragée caramels fruitcake jelly-o. Gummies cookie danish jelly soufflé chupa chups. Cheesecake pudding oat cake toffee. Toffee danish macaroon marshmallow gingerbread gummies. Muffin soufflé ice cream jelly beans. Wafer cheesecake cupcake cookie carrot cake I love jujubes sesame snaps. Sweet roll gummies cake tiramisu jelly beans pastry carrot cake. I love sweet halvah I love candy cookie tiramisu chocolate.',
  '2225 N Monroe St, Tallahassee, FL 32303, USA', -- location
  '30.470386', -- lat
  '-84.287964', -- lon
  'PRI', -- accessibility.
  31, -- user id, some FSU student 
  0, -- rso id. 
  2-- university id, FSU 
);

CALL create_event(
  'FSU Event 5', -- name
  '2017-07-22 10:00:00', -- start time -- this overlaps an earlier event, but is in a different location.
  '2017-07-22 11:00:00', -- end time
  '123-456-7890', -- telephone
  'pmappleleaf@noles.flsu.edu', -- email
  -- description written in cupcake ipsum
  'Candy cookie candy cheesecake jelly-o. Pastry jelly tiramisu sugar plum jelly bear claw candy candy canes dragée. Marzipan topping caramels croissant. Muffin oat cake lollipop croissant pudding I love I love I love. Ice cream gummi bears cookie topping cake chocolate jelly beans carrot cake marshmallow. Bear claw cake marzipan cake cookie bear claw sweet. Lollipop carrot cake donut. Carrot cake I love bear claw tiramisu tiramisu jelly beans dragée pastry. I love pastry ice cream tootsie roll ice cream carrot cake icing cake. Topping I love sugar plum gummi bears cake.',
  '600 W College Ave, Tallahassee, FL 32306, USA', -- location
  '30.441878', -- lat
  '-84.298489', -- lon
  'PUB', -- accessibility. since it's public. it will be seen by members of other schools.
  31, -- user id, some FSU student 
  0, -- rso id. 
  2-- university id, FSU 
);

COMMIT;
