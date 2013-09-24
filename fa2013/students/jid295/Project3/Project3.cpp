/* 
 * EE312 Simple CRM Project
 *
 * Joshua Dong
 * 
 */

#include <stdio.h>
#include <assert.h>
#include "String.h"
#include "Invent.h"

#define MAX_CUSTOMERS 1000

typedef enum {
    diapers, bottles, rattles
} StoreItem;

Customer customers[MAX_CUSTOMERS];
int num_bottles = 0;
int num_diapers = 0;
int num_rattles = 0;
int num_customers = 0;

StoreItem checkItem(String item) {
    String Bottles = StringCreate("Bottles");
    String Diapers = StringCreate("Diapers");
    String Rattles = StringCreate("Rattles");
    StoreItem item_enum;
    if (StringIsEqualTo(&item, &Bottles)) {
        item_enum = bottles;
    } else if (StringIsEqualTo(&item, &Diapers)) {
        item_enum = diapers;
    } else if (StringIsEqualTo(&item, &Rattles)) {
        item_enum = rattles;
    }
    StringDestroy(&Bottles);
    StringDestroy(&Diapers);
    StringDestroy(&Rattles);
    return item_enum;
}

void updateCustomer(String customer_name, StoreItem item, int count) {
    int customer_index = 0;
    while (customers[customer_index].name.len > 0) {
        if (StringIsEqualTo(&(customers[customer_index].name), &customer_name)) {
            switch (item) {
                case bottles:
                    customers[customer_index].bottles += count;
                case diapers:
                    customers[customer_index].diapers += count;
                case rattles:
                    customers[customer_index].rattles += count;
                default:
                    ;
            }
            return;
        }
        ++customer_index;
    }
    Customer new_client;
    new_client.name = customer_name;
    new_client.bottles = 0;
    new_client.diapers = 0;
    new_client.rattles = 0;
    switch (item) {
	case bottles:
            new_client.bottles = count;
	case diapers:
            new_client.diapers = count;
	case rattles:
            new_client.rattles = count;
	default:
	    ;
    }
    customers[customer_index] = new_client;
    num_customers = 0;
}

/* clear the inventory and reset the customer database to empty */
void reset(void) {
    for (int i = 0; i < MAX_CUSTOMERS; ++i) {
        //StringDestroy(&(customers[i].name));
        customers[i].bottles = 0;
        customers[i].diapers = 0;
        customers[i].rattles = 0;
    }
    num_bottles = 0;
    num_diapers = 0;
    num_rattles = 0;
    num_customers = 0;
}

void processSummarize() {
    printf("There are %d diapers, %d bottles and %d rattles in inventory\n", num_diapers, num_bottles, num_rattles);
    printf("we have had a total of %d different customers\n", num_customers);
    return;
}

void processPurchase() {
    String customer_name;
    String item;
    signed int count;
    bool valid_purchase = false;
    readString(&customer_name);
    readString(&item);
    readNum(&count);
    switch (checkItem(item)) {
        case bottles:
            if (num_bottles >= count) {
                num_bottles -= count;
                valid_purchase = true;
            }
            break;
        case diapers:
            if (num_diapers >= count) {
                num_diapers -= count;
                valid_purchase = true;
            }
            break;
        case rattles:
            if (num_rattles >= count) {
                num_rattles -= count;
                valid_purchase = true;
            }
            break;
    }
    if (valid_purchase) {
        updateCustomer(customer_name, checkItem(item), count);
    } else {
        printf("Sorry ");
        StringPrint(&customer_name);
        switch (checkItem(item)) {
            case bottles:
                printf(", we only have %d Bottles\n", num_bottles);
                break;
            case diapers:
                printf(", we only have %d Diapers\n", num_diapers);
                break;
            case rattles:
                printf(", we only have %d Rattles\n", num_rattles);
                break;
        }
    }
    StringDestroy(&customer_name);
    StringDestroy(&item);
}

void processInventory() {
    String item;
    int count;
    readString(&item);
    readNum(&count);
    switch (checkItem(item)) {
        case bottles:
            num_bottles += count;
            break;
        case diapers:
            num_diapers += count;
            break;
        case rattles:
            num_rattles += count;
            break;
    }
    StringDestroy(&item);
}
