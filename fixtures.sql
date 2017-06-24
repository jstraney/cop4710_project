BEGIN;

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
INSERT INTO universities (name, location, lat, lon, email_domain, website_url)
VALUES
("University of Central Florida", "4000 Central Florida Blvd, Orlando, FL 32816", 28.602427, -81.200060, "@knights.ucf.edu", "http://www.ucf.edu"),
("Florida State University", "600 W College Ave, Tallahassee, FL 32306", 30.441878, -84.298489, "@noles.fsu.edu", "http://www.fsu.edu"),
("University of Kansas", "1450 Jayhawk Blvd, Lawrence, KS 66045", 38.954344, -95.255796, "@kansas.ppl.edu", "http://www.kans.edu"),
("University of Michigan", "500 S State St, Ann Arbor, MI 48109", 42.278044, -83.738224, "@mich.ppl.edu", "http://www.mich.edu"),
("Cornell University", "Ithaca, NY 14850", 42.453449 , -76.473503, "@corn.ppl.edu", "http://www.corn.edu"),
("Yale University", "New Haven, CT 06520", 41.316324, -72.922343, "@yale.ppl.edu", "http://www.yale.edu");

-- all users have a password of 1234
INSERT INTO users (first_name, last_name, email, hash, user_name, role)
VALUES
("jeff", "straney", "jeff@gmail.com", "$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", "jstraney", "SA"),
("joe", "bilabog", "bilabog@knights.ucf.edu", "$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", "jbilabog", "STU"),
("robert", "doorstopper", "doorstopper@knights.ucf.edu", "$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", "rdoorstopper", "STU"),
("raymond", "faust", "faust@knights.ucf.edu", "$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", "rfaust", "STU"),
("ross", "olson", "rolson@knights.ucf.edu", "$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", "rolson", "STU"),
("rafael", "caprese", "rafael@knights.ucf.edu", "$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", "rafaelthepainter", "STU"),
("rachel", "primrose", "rprimrose@knights.ucf.edu", "$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", "primrose", "STU"),
("robert", "sandcastle", "sandcastle@yaleppl.yale.edu", "$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", "rsandcastle", "STU"),
("josh", "doorstopper", "doorstopper@noles.fsu.edu", "$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", "jdoorstopper", "STU"),
("jonas", "borg", "jborg@kans.ppl.edu", "$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", "jonasborg", "STU"),
("jenifer", "bosch", "jbosch@mich.ppl.edu", "$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", "jbosch", "STU"),
("travis", "tarmach", "travis@mich.ppl.edu", "$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", "ttarmach", "ADM"),
("sara", "mackley", "sara@knights.ucf.edu", "$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", "smackley", "ADM");

INSERT INTO attending (user_id, uni_id)
VALUES
(2, 1),
(3, 1),
(4, 1),
(5, 1),
(6, 1),
(7, 1),
(8, 3),
(9, 2),
(10, 2),
(11, 6);

INSERT INTO affiliated_with(user_id, uni_id)
VALUES
(12, 4),
(13, 1);

COMMIT;
