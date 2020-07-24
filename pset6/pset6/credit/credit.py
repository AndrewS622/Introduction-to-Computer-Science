# Get user input
card_num = str(input("Number: "))

# Initialize sum counter and get length
card_sum = 0
length = len(card_num)

# Loop through each digit
# j indexes the position from the right-hand side
# If on an even position, add the digits of twice the jth digit to the sum
# Otherwise, add the digit to the sum
for i in range(length):
    j = length - i - 1
    if not i % 2:
        card_sum += int(card_num[j])
    else:
        digit = 2 * int(card_num[j])
        if digit > 9:
            card_sum += (1 + digit % 10)
        else:
            card_sum += digit

# Check the sum for validity, and if valid, check the first one or two digits and the length
if card_sum % 10:
    print("INVALID")
elif length == 15 and (card_num[0:2] == "34" or card_num[0:2] == "37"):
    print("AMEX")
elif length == 16 and int(card_num[0:2]) > 50 and int(card_num[0:2]) <= 55:
    print("MASTERCARD")
elif length == 13 or length == 16 and card_num[0] == "4":
    print("VISA")
else:
    print("INVALID")