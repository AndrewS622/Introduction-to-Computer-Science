import cs50
from sys import argv
import csv


def main():
    # Check for correct number of input arguments
    if not len(argv) == 2:
        print("Usage: python roster.py housename")
        return 1
    housename = argv[1]

    # Start communication with database
    db = cs50.SQL("sqlite:///students.db")

    # Search database for all students in specified house
    students = db.execute(f"SELECT first, middle, last, birth FROM students WHERE house = '{housename}' ORDER BY last, first")

    # Print each student row-by-row, accounting for possibility of no middle name
    for row in students:
        if row['middle'] is None:
            print("{} {}, born {}".format(row['first'], row['last'], row['birth']))
        else:
            print("{} {} {}, born {}".format(row['first'], row['middle'], row['last'], row['birth']))


main()