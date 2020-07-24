# Make sure user inputs a value containing no more than one decimal point and is greater than or equal to zero
while True:
    change = input("Change owed: ")
    if change.replace(".", "", 1).isdigit():
        if float(change) >= 0:
            break

# Initialize coin counter and convert float to integer number of cents by rounding away decimal places after multiplication by 100
num_coin = 0
remaining = round(100*float(change))

# Decrement value greedily, choosing largest coin value smaller or equal to remaining value and incrementing counter until value remaining is zero
while remaining > 0:
    if remaining >= 25:
        num_coin += 1
        remaining -= 25
    elif remaining >= 10:
        num_coin += 1
        remaining -= 10
    elif remaining >= 5:
        num_coin += 1
        remaining -= 5
    else:
        num_coin += 1
        remaining -= 1

# Print result
print(num_coin)