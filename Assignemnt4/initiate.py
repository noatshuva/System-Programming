import sqlite3  # importing the sqlite3 library will result in a sqlite3 variable
import os  # Just for os.path.isfile(...
import sys

# Check if DB already exists BEFORE calling the connect, since connect will create it
DBExist = os.path.isfile('moncafe.db')
if DBExist:
    os.remove('moncafe.db')

# Will create a connection to 'moncafe.db' file. If the file does not exist, it will create it
dbcon = sqlite3.connect('moncafe.db')
cursor = dbcon.cursor()


# Define a function to be called when the interpreter terminates
def close_db():
    dbcon.commit()
    dbcon.close()


# register close_db to be called when the interpreter terminates
# atexit.register(close_db)


def create_tables():
    # this table holds information of the suppliers
    cursor.execute(""" CREATE TABLE Suppliers(id INTEGER PRIMARY KEY,
                                                name TEXT NOT NULL,
                                                contact_information TEXT )""")
    # this table holds information of the products
    cursor.execute(""" CREATE TABLE Products(id INTEGER PRIMARY KEY,
                                                description TEXT NOT NULL,
                                                price REAL NOT NULL,
                                                quantity INTEGER NOT NULL )""")
    # this table holds information of the coffee-stands
    cursor.execute(""" CREATE TABLE Coffee_stands(id INTEGER PRIMARY KEY,
                                                    location TEXT NOT NULL,
                                                    number_of_employees INTEGER)""")
    # this table holds information of the employees
    cursor.execute(""" CREATE TABLE Employees(id INTEGER PRIMARY KEY ,
                                                 name TEXT NOT NULL,
                                                 salary REAL NOT NULL,
                                                 coffee_stand INTEGER REFERENCES Coffee_stands)""")
    # this table holds information of all activities of the chain including sells and deliveries
    cursor.execute(""" CREATE TABLE Activities(product_id INTEGER REFERENCES Products,
                                                  quantity INTEGER NOT NULL,
                                                  activator_id INTEGER NOT NULL,
                                                  date DATE NOT NULL )""")


# insert a record into Employees table
def insert_employee(id, name, salary, coffee_stand):
    cursor.execute("INSERT INTO Employees VALUES (?, ?, ?, ?)", [int(id), name, float(salary), int(coffee_stand)])


# insert a record into Suppliers table
def insert_supply(id, name, contact_information):
    cursor.execute("INSERT INTO Suppliers VALUES (?, ?, ?)", [int(id), name, contact_information])


# insert a record into Products table
def insert_product(id, description, price, quantity=0):
    cursor.execute("INSERT INTO Products VALUES (?, ?, ?, ?)", [int(id), description, float(price), int(quantity)])


# insert a record into Coffee_stands table
def insert_coffeestand(id, location, number_of_employees):
    cursor.execute("INSERT INTO Coffee_stands VALUES (?, ?, ?)", [int(id), location, int(number_of_employees)])


# insert a record into Activities table
def insert_activity(product_id, quantity, activator_id, date):
    cursor.execute("INSERT INTO Activities VALUES (?, ?, ?, ?)", [int(product_id), int(quantity), int(activator_id), int(date)])




def insert_data(config_file):
    config_content = open(config_file, "r")
    config_content_string = config_content.read()
    for line in config_content_string.split('\n'):
        details = line.split(', ')
        if details[0] == "E":
            if details.__len__() == 5:
                insert_employee(details[1].strip(), details[2].strip(), details[3].strip(), details[4].strip())
        if details[0] == "S":
            if details.__len__() == 4:
                insert_supply(details[1].strip(), details[2].strip(), details[3].strip())
        if details[0] == "P":
            if details.__len__() == 4:
                insert_product(details[1].strip(), details[2].strip(), details[3].strip())
        if details[0] == "C":
            if details.__len__() == 4:
                insert_coffeestand(details[1].strip(), details[2].strip(), details[3].strip())


def main(args):
    fileinput = args[1]
    create_tables()
    insert_data(fileinput)
    close_db()


if __name__ == '__main__':
    main(sys.argv)


