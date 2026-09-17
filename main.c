#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_PATIENTS 100
#define NUMS_SPECIALTIES 4
#define NUM_WARDS 4
int p_id[MAX_PATIENTS];
char p_name[MAX_PATIENTS][50];
int p_age[MAX_PATIENTS];
int p_urgency[MAX_PATIENTS];
int p_specialty_id[MAX_PATIENTS];
int p_is_admitted[MAX_PATIENTS];
int p_ward_id[MAX_PATIENTS];
int p_bed_num[MAX_PATIENTS];
int p_days_admitted[MAX_PATIENTS];
int patient_count = 0;
int bedOccupancy[4][20] = {0};
void display_triage_queue();
void generate_analytics();
void register_patient();
void mainmenue();
int main()
{
    mainmenue();
    return 0;
}
void mainmenue()
{  int choice;
    printf("===================================================\n");
    printf("    SMART HOSPITAL MANAGEMENT SYSTEM - MENU\n");
    printf("===================================================\n");
    printf("1.Register New Patient Intake\n");
    printf("2.View Emergency Triage Priority Queue\n");
    printf("3.Display Analytics & Performance Report\n");
    printf("Enter Choice(1-3): ");
    scanf("%d",&choice);
    if(choice==1)
    {
      register_patient();
    }
    else if (choice==2)
    {
     display_triage_queue();
    }
    else if (choice==3)
    {
      generate_analytics();
    }
    else
    {
     printf("Invalid option.Try again.\n");
    }


}
void register_patient()
{ if (patient_count >= MAX_PATIENTS)
    {
    printf("System capacity reached!\n");
    return;
    }


}
void display_triage_queue()
{

}
void generate_analytics()
{

}


