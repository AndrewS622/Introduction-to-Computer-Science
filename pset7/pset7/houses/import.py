import cs50
from sys import argv
import csv


def main():
    # Check for correct number of input arguments
    if not len(argv) == 2:
        print("Usage: python import.py characters.csv")
        return 1

    # Create database by opening and closing an empty file and start communication with database
    open(f"students.db", "w").close()
    db = cs50.SQL("sqlite:///students.db")

    # Create table with first, middle, last name, house, and birth columns
    db.execute("CREATE TABLE students (first TEXT, middle TEXT, last TEXT, house TEXT, birth NUMERIC)")

    # Open csv file and read in each row
    with open(argv[1]) as csvfile:
        dic = csv.DictReader(csvfile)

        for row in dic:
            # Parse each row into name, house, and birth
            name = row['name']
            house = row['house']
            birth = row['birth']

            # Parse name into first, middle, last or first, last depending on size of str.split()
            if len(name.split()) == 2:
                first = name.split()[0]
                middle = None
                last = name.split()[1]
            elif len(name.split()) == 3:
                first = name.split()[0]
                middle = name.split()[1]
                last = name.split()[2]
            else:
                print("Name error: {}".format(name))
                return 2

            # Write student into table
            db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)",
                       first, middle, last, house, birth)


main()