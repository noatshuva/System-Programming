import os
import sqlite3

# Check if DB already exists BEFORE calling the connect, since connect will create it
DBExist = os.path.isfile('moncafe.db')
# Will create a connection to 'moncafe.db' file. If the file does not exist, it will create it
dbcon = sqlite3.connect('moncafe.db')
cursor = dbcon.cursor()


# Define a function to be called when the interpreter terminates
def close_db():
    dbcon.commit()
    dbcon.close()

def print_generic(table_name, order_by="id"):
    cursor.execute("SELECT * FROM " + table_name + " ORDER BY " + order_by)
    my_list = cursor.fetchall()
    print(table_name.replace("_", " "))
    for item in my_list:
        print(item)


def print_employees_report():
    print("Employees report")
    cursor.execute('SELECT*FROM Employees ORDER by name')
    employeereport = cursor.fetchall()
    cursor.execute("SELECT * FROM Activities")
    actlist = cursor.fetchall()
    for empl in employeereport:
        income = 0
        for act in actlist:
            if empl[0] == act[2]:
                pId = act[0]
                cursor.execute("SELECT price FROM Products WHERE id={}".format(pId))
                price = cursor.fetchone()[0]
                income = income + abs(price * act[1])

        cursor.execute("SELECT location FROM Coffee_stands WHERE  id={}".format(empl[3]))
        location = cursor.fetchone()[0]
        print("{} {} {} {}".format(empl[1], empl[2], location, income))


def print_activities_report():
    cursor.execute("""SELECT Activities.date, Products.description, Activities.quantity, Employees.name, Suppliers.name 
                      FROM Activities JOIN Products ON Activities.product_id == Products.id
                                      LEFT JOIN Suppliers ON Activities.activator_id = Suppliers.id
                                      LEFT JOIN Employees ON Activities.activator_id = Employees.id
                                      ORDER BY Activities.date""")

    activitiesreport = cursor.fetchall()
    if activitiesreport:
        print("\nActivities")
        for act in activitiesreport:
            print(act)


def main():
    order_by_list = ["date"] + (["id"] * 4)
    table_names_list = ["Activities", "Coffee_stands", "Employees", "Products", "Suppliers"]
    for pair in zip(table_names_list, order_by_list):
        print_generic(pair[0], pair[1])
    print()
    print_employees_report()
    print_activities_report()
    close_db()


if __name__ == '__main__':
    main()
