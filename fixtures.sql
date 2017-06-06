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
