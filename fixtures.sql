-- this is an sql script which will insert test records
-- whenever we drop and re-upload our database.
-- It would be cool to write an 'install script' with the option of test records
-- but for now, you can just copy this query and paste it into phpMyadmin under the
-- SQL tab of the proper table.
INSERT INTO universities (uni_name, uni_street, uni_state, uni_zip, uni_num_students)
VALUES
("University of Central Florida", "University Blvd", "FL", "32816", 0),
("Florida State University", "Gainesville Blvd", "FL", "32222", 0),
("University of Kansas", "Kansas Rd", "KA", "43215", 0),
("University of Michigan", "Michigan St", "KA", "33516", 0),
("Cornell University", "Ithaca Ln", "NY", "33444", 0),
("Yale University", "Yale Blvd", "CT", "06520", 0);

-- all users have a password of 1234
INSERT INTO users (first_name, last_name, email, uni_id, hash, user_name, role)
VALUES
("jeff", "straney", "jeff@gmail.com", 1, "$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", "jstraney", "SA"),
("joe", "bilabog", "bilabog@knights.ucf.edu", 1, "$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", "jbilabog", "STU"),
("robert", "doorstopper", "doorstopper@knights.ucf.edu", 1, "$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", "rdoorstopper", "STU"),
("robert", "sandcastle", "sandcastle@yaleppl.yale.edu", 6, "$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", "rsandcastle", "STU"),
("josh", "doorstopper", "doorstopper@noles.fsu.edu", 2, "$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", "rsandcastle", "STU"),
("jonas", "borg", "jborg@kansas.edu", 3, "$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", "jonasborg", "STU"),
("jenifer", "bosch", "jbosch@michigan.edu", 4, "$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", "jbosch", "STU"),
("travis", "tarmach", "travis@michigan.edu", 4, "$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", "ttarmach", "ADM"),
("sara", "mackley", "jeff@gmail.com", 1, "$2y$12$QJpIgFQA42Lrsk6qjRXZuOTg8MMu4r.zOXfRhL4fKMOZIe8RS/DXq", "smackley", "ADM");
