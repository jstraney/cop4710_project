BEGIN;
-- reset auto-increments
ALTER TABLE `events`AUTO_INCREMENT = 1;
ALTER TABLE users AUTO_INCREMENT = 1;
ALTER TABLE universities AUTO_INCREMENT = 1;
ALTER TABLE commented_on AUTO_INCREMENT = 1;
ALTER TABLE events AUTO_INCREMENT = 1;

-- this is an sql script which will insert test records
-- whenever we drop and re-upload our database.
-- It would be cool to write an 'install script' with the option of test records
-- but for now, you can just copy this query and paste it into phpMyadmin under the
-- SQL tab of the proper table.
INSERT INTO universities (name, street, state, zip, email_domain, website_url)
VALUES
("University of Central Florida", "University Blvd", "FL", "32816", "@knights.ucf.edu", "http://www.ucf.edu"),
("Florida State University", "Gainesville Blvd", "FL", "32222", "@noles.fsu.edu", "http://www.fsu.edu"),
("University of Kansas", "Kansas Rd", "KA", "43215", "@kansas.ppl.edu", "http://www.kans.edu"),
("University of Michigan", "Michigan St", "KA", "33516", "@mich.ppl.edu", "http://www.mich.edu"),
("Cornell University", "Ithaca Ln", "NY", "33444", "@corn.ppl.edu", "http://www.corn.edu"),
("Yale University", "Yale Blvd", "CT", "06520", "@yale.ppl.edu", "http://www.yale.edu");

-- all users have a password of 1234
INSERT INTO users (first_name, last_name, email, hash, user_name, role)
VALUES
("jeff", "straney", "jeff@gmail.com", "$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", "jstraney", "SA"),
("joe", "bilabog", "bilabog@knights.ucf.edu", "$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", "jbilabog", "STU"),
("robert", "doorstopper", "doorstopper@knights.ucf.edu", "$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", "rdoorstopper", "STU"),
("robert", "sandcastle", "sandcastle@yaleppl.yale.edu", "$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", "rsandcastle", "STU"),
("josh", "doorstopper", "doorstopper@noles.fsu.edu", "$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", "jdoorstopper", "STU"),
("jonas", "borg", "jborg@kansas.edu", "$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", "jonasborg", "STU"),
("jenifer", "bosch", "jbosch@michigan.edu", "$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", "jbosch", "STU"),
("travis", "tarmach", "travis@michigan.edu", "$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", "ttarmach", "ADM"),
("sara", "mackley", "sara@gmail.com", "$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", "smackley", "ADM");
INSERT INTO attending (user_id, uni_id)
VALUES
(2, 1),
(3, 1),
(4, 3),
(5, 2),
(6, 2),
(7, 6);
COMMIT;
