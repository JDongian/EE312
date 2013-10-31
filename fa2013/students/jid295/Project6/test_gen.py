import re
import sys

inventory = {}
customers = {}

def execute(line):
    params = re.split('\s+', line)
    if params[0] == "Inventory":
        if params[1] in inventory.keys():
            inventory[params[1]] += params[2]
        else:
            inventory[params[1]] = params[2]
    elif params[0] == "Purchase":
        if params[1] in inventory.keys():
            inventory[params[1]] += params[2]
        else:
            print "Sorry %s, we only have %d %s\n" %\
                  (params[1], inventory[params[2]], params[2])
    elif params[0] == "Summarize":
        print "Done."

def crm(filename):
    with open(filename, 'r') as f_in:
        lines = re.findall("(.+?)[\n\r]+", f_in.read())
    for line in lines:
        if line == "Quit":
            break
        execute(line)

if __name__ == "__main__":
    crm("test1.txt")


test_case = """Inventory Bottles {bottles}
Inventory Rattles {rattles}
Inventory Diapers {diapers}

Purchase Craig Bottles {bottles_p}
Purchase Craig Rattles {rattles_p}
Purchase Craig Diapers {diapers_p}


Summarize
Quit
/** test1 Results should be: **/
There are {diapers_end} diapers, {bottles_end} bottles and {rattles_end} rattles in inventory
we have had a total of 1 different customers
Craig has purchased the most diapers (120)
Craig has puchased the most bottles ()
Craig has puchased the most bottles ()
no one has purchased any rattles
/********* end of test 1 *************/"""




