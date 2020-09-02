import sqlite3
import os
import sys

# Check if DB already exists BEFORE calling the connect, since connect will create it
import printdb

DBExist = os.path.isfile('moncafe.db')

# Will create a connection to 'moncafe.db' file. If the file does not exist, it will create it
dbcon = sqlite3.connect('moncafe.db')
cursor = dbcon.cursor()


# Define a function to be called when the interpreter terminates
def close_db():
    dbcon.commit()
    dbcon.close()


# register close_db to be called when the interpreter terminates


def insert_activity(fileinput):
    config_file = open(fileinput, "r")
    config_file_string = config_file.read()
    lines = config_file_string.split('\n')
    for line in lines:
        details = line.split(', ')
        if int(details[1]) < 0:
            cursor.execute("SELECT quantity FROM Products WHERE id = " + details[0].strip())
            current_quantity = cursor.fetchall()
            if int(details[1]) * (-1) <= current_quantity[0][0]:
                add_activity(details[0].strip(), details[1].strip(), details[2].strip(), details[3].strip())
                new_quantity(details[0].strip(), details[1].strip())
        elif int(details[1]) > 0:
            add_activity(details[0].strip(), details[1].strip(), details[2].strip(), details[3].strip())
            new_quantity(details[0].strip(), details[1].strip())


def add_activity(product_id, quantity, activator_id, date):
    cursor.execute("INSERT INTO Activities VALUES (?,?,?,?)", [product_id, quantity, activator_id, date])


def new_quantity(product_id, quantity):
    cursor.execute("UPDATE Products SET quantity = quantity +" + quantity + " WHERE id =" + product_id)


def main(args):
    file = args[1]
    insert_activity(file)
    close_db()


if __name__ == '__main__':
    main(sys.argv)
    printdb.main()
