from sys import argv
import csv


def main():
    # Check for correct number of input arguments
    if not len(argv) == 3:
        print("Usage: python dna.py STRcounts.csv DNAsequence.txt")
        return 1

    # Open sequence .txt file and save sequence as a list
    f = open(argv[2], "r")
    sequence = f.read()
    f.close()

    # Open dictionary file, import into DictReader object, extract field names, and initialize test dictionary for sequence
    with open(argv[1]) as csvfile:
        dic = csv.DictReader(csvfile)
        fields = dic.fieldnames
        test_dict = {}
        for key in fields:
            if key == 'name':
                test_dict[key] = 'test'
            else:
                test_dict[key] = 0

        # Iterate through each base in the sequence as the start of a new reading frame and check against each STR in the database
        # If field is name, continue; otherwise, call find_STR function with current remaining sequence and STR
        # If returned value is greater than that currently stored, update test dictionary with new value
        for i in range(len(sequence)):
            for key in fields:
                if key == 'name':
                    continue
                count = find_STR(key, sequence[i:len(sequence)])
                if count > test_dict[key]:
                    test_dict[key] = count

        # Iterate through DictReader object, and compare each (non-name) field between the sequence results and the database
        # If values are equal, increment counter
        # Print name and exit if match is found (indicated by counter being equal to number of non-name fields)
        # Otherwise, print no match
        for row in dic:
            count_equal = 0
            for key in fields:
                if key == 'name':
                    continue
                if int(row[key]) == test_dict[key]:
                    count_equal += 1
            if count_equal == len(fields) - 1:
                print(row['name'])
                return 0
        print("No match")
        return 0


def find_STR(STR, sequence):
    ctr = 0
    i = 0
    l = len(STR)
    l_seq = len(sequence)
    while i + l < l_seq:
        if sequence[i:(i + l)] == STR:
            ctr += 1
            i += l
        else:
            return ctr
    return ctr


main()