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

#define offsetof(st, m) ((size_t)(&((st *)0)->m))
#define MAX_CUSTOMERS 1000

Customer BabyStore;
Customer customers[MAX_CUSTOMERS];
int num_customers = 0;

/* Returns the offset of the member value given a String.
 * Offset in is bytes.
 */
int getCustomerData(String item) {
    /* Conversion from string constant to char* is deprecated. */
    const char* bottles = "Bottles";
    const char* diapers = "Diapers";
    const char* rattles = "Rattles";
    String Bottles = StringCreate((char*)bottles);
    String Diapers = StringCreate((char*)diapers);
    String Rattles = StringCreate((char*)rattles);
    /* In the event of bad input, returns -1. */
    int offset = -1;
    if (StringIsEqualTo(&item, &Bottles)) {
        offset = offsetof(Customer, bottles);
    } else if (StringIsEqualTo(&item, &Diapers)) {
        offset = offsetof(Customer, diapers);
    } else if (StringIsEqualTo(&item, &Rattles)) {
        offset = offsetof(Customer, rattles);
    }
    StringDestroy(&Bottles);
    StringDestroy(&Diapers);
    StringDestroy(&Rattles);
    return offset;
}

/* Adds a purchase to the customers array. */
void updateCustomer(String customer_name, String item, int count) {
    /* Check if the customer is already in the array. */
    for (int i = num_customers-1; i >= 0; --i) {
        if (StringIsEqualTo(&customer_name, &(customers[i].name))) {
            *((int*)((char*)&customers[i] + getCustomerData(item))) += count;
            return;
        }
    }
    /* Add a new customer to the array. */
    Customer new_client;
    new_client.name = StringDup(&customer_name);
    new_client.bottles = 0;
    new_client.diapers = 0;
    new_client.rattles = 0;
    *((int*)((char*)&new_client + getCustomerData(item))) += count;
    customers[num_customers++] = new_client;
}

/* Clear the inventory and reset the customer database to empty */
void reset(void) {
    //Avoid freeing unallocated memory.
    //StringDestroy(&(BabyStore.name));
    BabyStore.bottles = 0;
    BabyStore.diapers = 0;
    BabyStore.rattles = 0;
    for (int i = 0; i < num_customers; ++i) {
        StringDestroy(&(customers[i].name));
        customers[i].bottles = 0;
        customers[i].diapers = 0;
        customers[i].rattles = 0;
    }
    num_customers = 0;
}

void processSummarize() {
    printf("There are %d diapers, %d bottles and %d rattles in inventory\n",
           BabyStore.diapers, BabyStore.bottles, BabyStore.rattles);
    printf("we have had a total of %d different customers\n", num_customers);
    Customer records;
    records.bottles = 0;
    records.diapers = 0;
    records.rattles = 0;
    String* recordHolders[3];
    for (int i = num_customers-1; i >= 0; --i) {
        if (customers[i].bottles > records.bottles) {
            recordHolders[0] = &(customers[i].name);
            records.bottles = customers[i].bottles;
        }
        if (customers[i].diapers > records.diapers) {
            recordHolders[1] = &(customers[i].name);
            records.diapers = customers[i].diapers;
        }
        if (customers[i].rattles > records.rattles) {
            recordHolders[2] = &(customers[i].name);
            records.rattles = customers[i].rattles;
        }
    }
    /* In the case of a tie, pick the first customer. */
    if (records.diapers != 0) {
        StringPrint(recordHolders[1]);
        printf(" has purchased the most diapers (%d)\n", records.diapers);
    } else {
        printf("no one has purchased any diapers.\n");
    }
    if (records.bottles != 0) {
        StringPrint(recordHolders[0]);
        printf(" has purchased the most bottles (%d)\n", records.bottles);
    } else {
        printf("no one has purchased any bottles.\n");
    }
    if (records.rattles != 0) {
        StringPrint(recordHolders[2]);
        printf(" has purchased the most rattles (%d)\n", records.rattles);
    } else {
        printf("no one has purchased any rattles.");
    }
}

void processPurchase() {
    String customer_name;
    String item;
    int count;
    readString(&customer_name);
    readString(&item);
    readNum(&count);
    if (*((int*)((char*)&BabyStore + getCustomerData(item))) >= count) {
        *((int*)((char*)&BabyStore + getCustomerData(item))) -= count;
        updateCustomer(customer_name, item, count);
    } else {
        printf("Sorry ");
        StringPrint(&customer_name);
        printf(", we only have %d ",
               *((int*)((char*)&BabyStore + getCustomerData(item))));
        StringPrint(&item);
        printf("\n");
    }
    StringDestroy(&customer_name);
    StringDestroy(&item);
}

/* Add items to inventory.
 * Expects positive values.
 */
void processInventory() {
    String item;
    int count;
    readString(&item);
    readNum(&count);
    *((int*)((char*)&BabyStore + getCustomerData(item))) += count;
    StringDestroy(&item);
}
