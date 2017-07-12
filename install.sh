# install script using bash shell scripting.
mysql -u root --password=""< event.sql

# procedures are included in the latest database pull. optionally, procedures.sql
# could be run, but its unnecessary as the procedures are stored in event.sql

# optionally, 
if [ "$1" == "--fixtures" ]
then

  # import test records
  mysql -u root --password=""< "fixtures.sql"

fi

if [ "$?" -eq 0 ]
then
  echo "Database successfully imported"
else
  echo "Something went wrong with the install"
fi
