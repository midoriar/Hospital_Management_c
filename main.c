/*******************************************************BEGENING OF THE TP*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
/*******************************************************We_start_by_the_structures_we_need*****************************************************************************/

// Structure for patient information
typedef struct patient
{
    char name[100];
    int age;
    char address[100];
    long phone;
    char bloodgroup[100];
    char gender;
    long ss_number; // social security number
    int priority;   // priority of the patient
} patient;

// Structure for a single node in a queue
typedef struct queuenode
{
    // every single node will have two things
    patient *val;           // first: the patient information
    struct queuenode *addr; // second : a pointer to the next node
} QueueNode;

// Structure for a  the head and the tail of the queue
typedef struct
{
    QueueNode *head, *tail;
} Queue;

// Structure for a node in the linked list
typedef struct listNode
{
    Queue data;            // each node contain a queue
    struct listNode *next; // pointer to the- next node
} ListNode;
/**********************************************************************end of the declaration*********************************************************************/

/***********************************************************************MENU PART**************************************************************************************/

void deparmenu() // to chose the department .
{
    printf("\n");
    printf("\t\t\033[1;36mPlease refer to this table for your medical condition!\033[0m\n");
    printf("\t\t\033[1;36m1. Cardiology\n");
    printf("\t\t2. Chirurgical (Surgery)\n");
    printf("\t\t3. Orthopedics\n");
    printf("\t\t4. Internal Medicine\n");
    printf("\t\t5. Otorhinolaryngology (ENT)\033[0m\n");
}
/*********************************************************************************************************************************/
void delete_menu() // to ask the user from where he want delete the patient
{
    printf("****************************choose what do you want to delete: ****************************\n");
    printf("1) delete a patient from the main service\n");
    printf("2) delete a patient from a special department\n");
}
/*********************************************************************************************************************************/
void display_menu() // we use it to ask the user from where he want to display the patient
{
    printf("****************************display mode: ****************************\n");
    printf("1) Display patient information from the main service\n");
    printf("2) Display patients of each department\n");
}
/********************************************************************END OF MENU PART**********************************************************************************/

/**************************************************************abstract machine  to assgine patient information**************************************************************/

void ass_first_name(patient *p, char *first_name) // assigning the first name
{
    // WE copy at most sizeof(p->name) - 1 characters from first_name to p->name
    //  The -1 ensures space for the null terminator
    strncpy(p->name, first_name, sizeof(p->name) - 1);
    p->name[sizeof(p->name) - 1] = '\0'; // Ensure null-termination
}
/***************************************************************************************************************************/
void ass_age(patient *p, int age) // assigning the age
{
    p->age = age;
}
/***************************************************************************************************************************/
void ass_address(patient *p, char *address) // assigning the address .
{
    // the same that we used in assigning the first name
    strncpy(p->address, address, sizeof(p->address) - 1);
    p->address[sizeof(p->address) - 1] = '\0'; // Ensure null-termination
}
/***************************************************************************************************************************/
void ass_phone(patient *p, long phone) // assigning the phone number .
{
    p->phone = phone;
}
/***************************************************************************************************************************/
void ass_bloodgroup(patient *p, char *bloodgroup) // assigning the blood group .
{
    strncpy(p->bloodgroup, bloodgroup, sizeof(p->bloodgroup) - 1); // Also the same way used before .
    p->bloodgroup[sizeof(p->bloodgroup) - 1] = '\0';               // Ensure null-termination
}
/***************************************************************************************************************************/
void ass_gender(patient *p, char gender) // assignig the gender .
{
    p->gender = gender;
}
/***************************************************************************************************************************/
void ass_ss_number(patient *p, long number) // assigning the social security number.
{
    p->ss_number = number;
}
/***************************************************************************************************************************/
void allocate(patient **ptr) // we use it to add a patient
{
    *ptr = (patient *)malloc(sizeof(patient));
    if (*ptr == NULL) // that means the allocation is failled
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE); // exit to stop progressing
    }
}
/***************************************************end of abstract machine  to assgine patient information****************************************************/

/**************************************************************************qeueus basic operations****************************************************************/

void CreateQueue(Queue *pf) // procedure_create_queue
{

    pf->head = NULL; // to create a queue we need to :
    pf->tail = NULL; // initilise the head and the tail by NULL
}
/***************************************************************************************************************************/
int EmptyQueue(Queue f)      // The_function_Empty_queue
{                            // it verify if the queue is empty or not
    return (f.head == NULL); // it will give 1 if true and 0 if else
}
/***************************************************************************************************************************/
int Enqueue(patient *x, Queue *pf)
{
    // Allocate memory for a new queue node
    QueueNode *p = (QueueNode *)malloc(sizeof(QueueNode));
    if (p == NULL)
        return 0;   // Return 0 if memory allocation fails
    p->val = x;     // Assign patient pointer to the new node's value
    p->addr = NULL; // Initialize the next pointer of the new node to NULL

    // Enqueue based on priority
    if (EmptyQueue(*pf) || x->priority > pf->tail->val->priority)
    {
        // If the queue is empty or the new patient has higher priority than the last patient in the queue
        if (!EmptyQueue(*pf))
            pf->tail->addr = p; // If not empty, link the previous tail to the new node
        pf->tail = p;           // Update the tail of the queue to the new node
        if (EmptyQueue(*pf))
            pf->head = p; // If the queue was empty, update the head to the new node
    }
    else
    {
        // If the new patient has lower or equal priority compared to some existing patients in the queue
        QueueNode *current = pf->head;
        while (current->addr != NULL && current->addr->val->priority >= x->priority)
            current = current->addr; // Traverse the queue until finding the right position
        p->addr = current->addr;     // Link the new node to the next node in the queue
        current->addr = p;           // Link the previous node to the new node
    }

    return 1; // Return 1 to indicate successful enqueue operation
}

/***************************************************************************************************************************/
// Modify the Dequeue function to dequeue based on priority
int Dequeue(patient **x, Queue *pf)
{
    // Declare necessary pointers for traversal and tracking
    QueueNode *p, *prev, *highestPriorityNode, *prevHighestPriorityNode;

    // Check if the queue is empty
    if (EmptyQueue(*pf))
        return 0; // Return 0 if the queue is empty

    // Initialize variables to track the highest priority node and its previous node
    highestPriorityNode = prevHighestPriorityNode = pf->head;
    prev = pf->head;
    p = pf->head->addr;

    // Traverse the queue to find the node with the highest priority
    while (p != NULL)
    {
        if (p->val->priority > highestPriorityNode->val->priority)
        {
            highestPriorityNode = p;        // Update highest priority node if found
            prevHighestPriorityNode = prev; // Update its previous node
        }
        prev = p; // Move to the next node
        p = p->addr;
    }

    // Dequeue the highest priority node
    *x = highestPriorityNode->val; // Retrieve the patient information
    if (highestPriorityNode == pf->head)
        pf->head = highestPriorityNode->addr; // Update head if dequeued node is the first node
    else
        prevHighestPriorityNode->addr = highestPriorityNode->addr; // Update previous node's link

    if (highestPriorityNode == pf->tail)
        pf->tail = prevHighestPriorityNode; // Update tail if dequeued node is the last node

    free(highestPriorityNode); // Free memory allocated for the dequeued node

    return 1; // Return 1 to indicate successful dequeue operation
}

/***********************************************************************end of qeueus basic operations*****************************************************************/

/***********************************************************************department list functions**********************************************************************/

ListNode *Departmentlist() // function to create a list of 5 node each node represents a depatment of the hospital
{
    ListNode *head = NULL;
    for (int i = 0; i < 5; i++) // loop to creat 5 nodes in the list
    {
        // Create a new node for each department
        ListNode *newNode = (ListNode *)malloc(sizeof(ListNode)); // allocate a node for the list
        if (newNode == NULL)
        {
            fprintf(stderr, "Memory allocation failed\n"); // check if there is no probleme with the allocation
            exit(EXIT_FAILURE);
        }

        CreateQueue(&(newNode->data)); // creat a queue inside a the node of the list
        newNode->next = head;          // Insert the new node into the list
        head = newNode;                // link the node to creat a list
    }

    return head;
}
/***************************************************************************************************************************/
void enqueueToDepartment(ListNode *head, int departmentIndex, patient *newPatient)
{
    int currentIndex = 1;
    ListNode *current = head;

    // Traverse to the specified department
    while (current != NULL && currentIndex < departmentIndex)
    {
        current = current->next;
        currentIndex++;
    }

    if (current == NULL)
    {
        printf("Department not found.\n");
        return;
    }

    // Enqueue the patient to the department's queue
    if (!Enqueue(newPatient, &(current->data)))
    {
        fprintf(stderr, "Failed to enqueue patient to department %d.\n", departmentIndex);
        exit(EXIT_FAILURE);
    }


}
/***************************************************************************************************************************/
void dequeueFromDepartment(ListNode *head, int departmentIndex, patient **dequeuedPatient)
{
    int currentIndex = 1;
    ListNode *current = head;

    // Traverse to the specified department
    while (current != NULL && currentIndex < departmentIndex)
    {
        current = current->next;
        currentIndex++;
    }

    if (current == NULL)
    {
        printf("Department not found.\n");
        return;
    }

    // Dequeue a patient from the department's queue
    if (!Dequeue(dequeuedPatient, &(current->data)))
    {
        printf("Department %d is empty.\n", departmentIndex);
        return;
    }

    printf("Patient dequeued from department %d:\n", departmentIndex);
    // Print patient information if needed
}

/****************************************************************** end of department list functions*******************************************************************/

/**********************************************************************MAIN OPERATOIN FUNCTIONS*************************************************************************/

void *insert(Queue *main_queue, ListNode *listnode) // A Procedure to get patient information and enqueue it into the main queue or a special department
{
    patient *ptr;   // create a pointer to a patient
    allocate(&ptr); // starting by allocating it

    printf("*----------------------------enter patient information------------------------------*\n");
    printf("Enter patient Name: "); // After we collect the information of the patient and assign it
    char name[100];
    scanf("%99s", name);
    ass_first_name(ptr, name); // assign the name

    printf("Enter the patient's age: ");
    int age;
    scanf("%d", &age);
    ass_age(ptr, age); // assign the age

    printf("Enter your home address: ");
    char address[100];
    scanf("%99s", address);
    ass_address(ptr, address); // assign the address

    printf("Enter your phone number: ");
    long phone;
    scanf("%ld", &phone);
    ass_phone(ptr, phone); // assign the phone number

    printf("Enter the blood group of Patient: ");
    char bloodgroup[100];
    scanf("%99s", bloodgroup);
    ass_bloodgroup(ptr, bloodgroup); // assign the blood group of the patient

    printf("Enter the gender(m for male and f for female): ");
    char gender;
    scanf(" %c", &gender);
    ass_gender(ptr, gender); // assign the gender

    printf("enter the social security number: ");
    long socialsecurity;
    scanf("%d", &socialsecurity);
    ass_ss_number(ptr, socialsecurity); // assign the social security number .

    // Prompt user for priority
    printf("Enter patient priority (1 to 10, where 10 is highest priority): ");
    scanf("%d", &(ptr->priority));
    printf("*--------------------------------------------------------------------------------*\n");

    printf("Do you want to choose a department or stay in the main service?\n");
    printf("***************************************\n");
    printf("               1: Choose department\n");
    printf("               2: Stay in main list\n");
    printf("***************************************\n");
    int choice;
    scanf("%d", &choice);
    // after the user choose we deal with his choice
    switch (choice)
    {
    case 1: // choosing a departement
        deparmenu();
        int dept_choice;
        do
        {
            printf("enter the department(between 1 and 5): ");
            scanf("%d", &dept_choice);
        } while (dept_choice <= 0 || dept_choice > 5);

        enqueueToDepartment(listnode, dept_choice, ptr); // enqeue the patient
        printf("Patient enqueued into department %d.\n", dept_choice);
        break;

    case 2: // if the user choose the main service
        Enqueue(ptr, main_queue);
        printf("Patient enqueued into main list.\n"); // to inform the user by that
        break;

    default:
        // if the user choose invalid choice
        printf("Invalid choice.\n");
        break;
    }
}
/****************************************************************************************************************************************/
ListNode *delete(ListNode *start, Queue *main_queue) // to delete a patient from the main service or one of the departments
{
    int choix;
    patient *patient = NULL; // Initialize patient pointer
    delete_menu();           // Added function call
    scanf("%d", &choix);
    switch (choix)
    {
    case 1:
        if (Dequeue(&patient, main_queue)) // Check if Dequeue operation is successful
        {
            printf("Patient dequeued from main service:\n"); // display the patient information for the last time
            printf("***************************************************\n");
            printf("*                 Patient Information              *\n");
            printf("***************************************************\n");
            printf("* %-14s: %-30s *\n", "Name", patient->name);
            printf("* %-14s: %-30d *\n", "Age", patient->age);
            printf("* %-14s: %-30s *\n", "Address", patient->address);
            printf("* %-14s: %-30ld *\n", "Phone", patient->phone);
            printf("* %-14s: %-30s *\n", "Blood Group", patient->bloodgroup);
            printf("* %-14s: %-30c *\n", "Gender", patient->gender);
            printf("* %-14s: %-30ld *\n", "Social Security Number", patient->ss_number);
            printf("***************************************************\n\n");
            free(patient); // Free memory allocated for the patient
        }
        else
        { // we inform the user that the main service is empty
            printf("Main service is empty.\n");
        }
        break;

    case 2:
    {
        int index;
        deparmenu();
        printf("Choose the department: \n");
        scanf("%d", &index);
        dequeueFromDepartment(start, index, &patient); // the same thing for the delete from a department
        break;
    }

    default: // when the user's choice doesn't exist .
        printf("Invalid choice.\n");
        break;
    }

    return start; // Added return statement
}
/*************************************************************************************************************************************************/
void displaypatient(Queue f, ListNode *departments)
{
    QueueNode *p;
    int choice;
    display_menu();
    printf("Enter your choice: ");
    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
        if (EmptyQueue(f))
        {
            printf("Queue is empty.\n");
            return;
        }
        p = f.head;
        printf("Main Queue elements:\n");
        while (p != NULL)
        {
            printf("***************************************************\n");
            printf("*                 Patient Information              *\n");
            printf("***************************************************\n");
            printf("* %-14s: %-30s *\n", "Name", p->val->name);
            printf("* %-14s: %-30d *\n", "Age", p->val->age);
            printf("* %-14s: %-30s *\n", "Address", p->val->address);
            printf("* %-14s: %-30ld *\n", "Phone", p->val->phone);
            printf("* %-14s: %-30s *\n", "Blood Group", p->val->bloodgroup);
            printf("* %-14s: %-30c *\n", "Gender", p->val->gender);
            printf("* %-14s: %-30ld *\n", "Social Security Number", p->val->ss_number);
            printf("* %-14s: %-30d *\n", "Priority", p->val->priority); // Display priority
            printf("***************************************************\n\n");
            p = p->addr;
        }
        break;

    case 2:
        if (departments == NULL)
        {
            printf("No departments found.\n");
            return;
        }
        int departmentIndex;
        printf("Enter department index (1 to 5): ");
        deparmenu();
        scanf("%d", &departmentIndex);

        if (departmentIndex < 1 || departmentIndex > 5)
        {
            printf("Invalid department index.\n");
            return;
        }

        ListNode *current = departments;
        int currentIndex = 1;
        while (current != NULL && currentIndex < departmentIndex)
        {
            current = current->next;
            currentIndex++;
        }

        if (current == NULL)
        {
            printf("Department not found.\n");
            return;
        }

        p = current->data.head;
        printf("Department %d elements:\n", departmentIndex);
        while (p != NULL)
        {
            printf("***************************************************\n");
            printf("*                 Patient Information              *\n");
            printf("***************************************************\n");
            printf("* %-14s: %-30s *\n", "Name", p->val->name);
            printf("* %-14s: %-30d *\n", "Age", p->val->age);
            printf("* %-14s: %-30s *\n", "Address", p->val->address);
            printf("* %-14s: %-30ld *\n", "Phone", p->val->phone);
            printf("* %-14s: %-30s *\n", "Blood Group", p->val->bloodgroup);
            printf("* %-14s: %-30c *\n", "Gender", p->val->gender);
            printf("* %-14s: %-30d *\n", "Social Security Number", p->val->ss_number);
            printf("* %-14s: %-30d *\n", "Priority", p->val->priority); // Display priority
            printf("***************************************************\n\n");
            p = p->addr;
        }
        break;

    default:
        printf("Invalid choice.\n");
        break;
    }
}

/*******************************************************************************************************************************************************/
void searchBySSNumber(long ss_number, Queue main_queue, ListNode *departments) // procedure to search for a patient using social security number in the hospital
{
    QueueNode *p;
    int found = 0; // a boolean variable initialized by zero

    // Search in the main queue
    p = main_queue.head; // to start manipulating the list
    while (p != NULL)
    {
        if (p->val->ss_number == ss_number)
        { // verify the condition
            // we display his informations
            printf("Patient found in the main queue:\n");
            printf("***************************************************\n");
            printf("*                 Patient Information              *\n");
            printf("***************************************************\n");
            printf("* %-14s: %-30s *\n", "Name", p->val->name);
            printf("* %-14s: %-30d *\n", "Age", p->val->age);
            printf("* %-14s: %-30s *\n", "Address", p->val->address);
            printf("* %-14s: %-30ld *\n", "Phone", p->val->phone);
            printf("* %-14s: %-30s *\n", "Blood Group", p->val->bloodgroup);
            printf("* %-14s: %-30c *\n", "Gender", p->val->gender);
            printf("* %-14s: %-30ld *\n", "Social Security Number", p->val->ss_number);
            printf("***************************************************\n");
            found = 1; // we set the variable in one to ensure that we found the patient
            break;
        }
        p = p->addr; // go to the next node
    }

    // Search in department queues
    ListNode *current = departments; // Initialize current to departments
    int i = 1;                       // Initialize i to 1
    char *departmentNames[] = {"Cardiology", "Chirurgical (Surgery)", "Orthopedics", "Internal Medicine", "Otorhinolaryngology (ENT)"};

    while (current != NULL)
    {                           // to manipulate the list
        p = current->data.head; // Initialize p inside the loop
        while (p != NULL)
        {
            if (p->val->ss_number == ss_number)
            { // that mean the condition is verified
                // we display the patient's information .
                printf("Patient found in department queue of department %s:\n", departmentNames[i - 1]); // Adjust index for department name
                printf("***************************************************\n");
                printf("*                 Patient Information              *\n");
                printf("***************************************************\n");
                printf("* %-14s: %-30s *\n", "Name", p->val->name);
                printf("* %-14s: %-30d *\n", "Age", p->val->age);
                printf("* %-14s: %-30s *\n", "Address", p->val->address);
                printf("* %-14s: %-30ld *\n", "Phone", p->val->phone);
                printf("* %-14s: %-30s *\n", "Blood Group", p->val->bloodgroup);
                printf("* %-14s: %-30c *\n", "Gender", p->val->gender);
                printf("* %-14s: %-30ld *\n", "Social Security Number", p->val->ss_number);
                printf("***************************************************\n");
                found = 1; // set the variable in one to ensure that we found him .
                break;
            }
            p = p->addr; // to the next .
        }
        if (found) // the boolean that we se before .
            break; // we stop progressing
        current = current->next;

        i++; // Increment i inside the outer loop to show the department name
    }

    if (!found)
    {
        // we didn't find him
        printf("Patient with social security number '%ld' not found.\n", ss_number);
    }
}
/*********************************************************************END OF MAIN OPERATOIN FUNCTIONS******************************************************************/

/************************************************************************TP INTRODUCTION*******************************************************************************/
void displayintro() // we used at the beginning
{
    printf("\n");
    printf("   \033[1;34m|=============================================================================================================================\033[0m\n");
    printf("   \033[1;34m|                                               High School of Computer Science Algiers                                       |\033[0m\n");
    printf("   \033[1;34m|                                                             TP1   ALSDD                                          2023/2024  |\033[0m\n");
    printf("   \033[1;34m|                                                                                                                             |\033[0m\n");
    printf("   \033[1;34m|     Realised by: \033[1;33mBENDIFALLAH RAMI, BENSAHA KHALIL\033[0m    \033[1;34mSection: A         Group: 02                                           \033[0m\n");
    printf("   \033[1;34m|=============================================================================================================================|\033[0m\n");
    printf("   \033[1;34m|                                                                                                                             |\033[0m\n");
    printf("   \033[1;31m|                                                   HOSPITAL ORGANISATION                                                     |\033[0m\n");
    printf("   \033[1;34m|=============================================================================================================================|\033[0m\n\n");
    printf("   \033[1;32mPress any key to go to the main menu...\033[0m");
}
/*******************************************************************************************************************************************************/
void loading()
{
    /*The principle is simple: we will just display the progress bar with the percentage,
       and then clear the screen and display the next bar.*/
    system("cls");

    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n");

    printf("\t\t\t\t|||                                                 5%%  ");
    Sleep(300);
    system("cls");

    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("\t\t\t\t|||||                                               20%%  ");
    Sleep(300);
    system("cls");

    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("\t\t\t\t||||||||||||||||                                    30%%  ");
    Sleep(300);
    system("cls");

    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("\t\t\t\t|||||||||||||||||||||||||                           45%%  ");
    Sleep(300);
    system("cls");

    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("\t\t\t\t|||||||||||||||||||||||||||||||                     60%%  ");
    Sleep(300);
    system("cls");

    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("\t\t\t\t||||||||||||||||||||||||||||||||||||||||            80%%  ");
    Sleep(300);
    system("cls");

    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("\t\t\t\t||||||||||||||||||||||||||||||||||||||||||||||||||  100%% ");
    system("cls");
}
/******************************************************************** END TP INTRODUCTION**************************************************************************/

/*************************************************************************THE MAIN PROGRAME***********************************************************************/

int main()
{

    displayintro(); // starting by the introduction mentioned before
    getchar();
    system("cls"); // to clear the screan
    loading();

    /* the following instructions used to create all the structures before
        to use it in the main programme                                    */
    patient *start = NULL;
    Queue main_queue;
    CreateQueue(&main_queue);

    ListNode *departments = NULL;
    departments = Departmentlist();
    int option;

    do
    {
        long ssnum;
        system("cls"); // clear screen

        // Main menu
        printf("   \033[1;36m"); // Set text color to bold cyan
        printf("   \t\t\t\t\t===============================================\n");
        printf("   \t\t\t\t\t|               \033[1;32m  HOSPITAL\033[0m                    |\n"); // Set text color to bold green for menu title
        printf("   \t\t\t\t\t===============================================\n");
        printf("   \t\t\t\t\t===============================================\n");
        printf("   \t\t\t\t\t|               \033[1;32m  MAIN MENU\033[0m                   |\n"); // Set text color to bold green for menu title
        printf("   \t\t\t\t\t===============================================\n");
        printf("   \t\t\t\t\t|  \033[1;32m1.\033[0m Add a Patient                           |\n"); // Set text color to bold green for menu option numbers
        printf("  \t\t\t\t\t|  \033[1;32m2.\033[0m Delete a Patient                        |\n");
        printf("  \t\t\t\t\t|  \033[1;32m3.\033[0m Display All patient                     |\n");
        printf("  \t\t\t\t\t|  \033[1;32m4.\033[0m Search for a Patient                    |\n");
        printf("   \t\t\t\t\t|  \033[1;32m5.\033[0m Exit                                    |\n");
        printf("   \t\t\t\t\t===============================================\n\n");
        printf("\033[0m"); // Reset text color to default

        printf("\t\t\t\t\t enter your choice: ");
        if (scanf("%d", &option) != 1)
        {                                                       // get the user's choice
            printf("                              \033[0;31m"); // Set text color to red
            printf("                              Invalid input. Please enter a number.\n");
            printf("                              \033[0m"); // Reset text color to default
            while (getchar() != '\n')
                ; // Clear input buffer
            continue;
        }
        system("cls");
        switch (option)
        { // treating the options witch is the choice
        case 1:
            // insert a patient .
            insert(&main_queue, departments);
            getchar();
            getchar();
            system("cls");
            break;
        case 2:
            // delete all the patient
            departments = delete (departments, &main_queue);
            printf("                              Press any key to return to the main menu...");
            getchar();
            getchar();
            system("cls");
            break;
        case 3:
            // display the patients from the main queue
            displaypatient(main_queue, departments);
            printf("                              Press any key to return to the main menu...");
            getchar();
            getchar();
            system("cls");
            break;
        case 4:
            // searching by the ss number
            printf("enter the social security number to search: ");
            scanf("%ld", &ssnum);
            searchBySSNumber(ssnum, main_queue, departments);
            printf("                              Press any key to return to the main menu...");
            getchar();
            getchar();
            system("cls");
            break;
        case 5:
            // to exit from the programme
            printf("\033[0;33m"); // Set text color to yellow
            printf("Exiting...\n");
            printf("\033[0m"); // Reset text color to default
            break;
        default:
            // when the option isn't in the menu displayed
            printf("\033[0;31m"); // Set text color to red
            printf("Invalid option. Please choose again.\n");
            printf("\033[0m"); // Reset text color to default
            break;
        }
    } while (option != 5);

    return 0;
}
/***************************************** THE END OF MAIN PROGRAM******************************************/
/**************************************************************THE END OF THE TP*****************************************************************************/
