#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_PATIENTS 100
#define NUM_SPECIALTIES 4
#define NUM_WARDS 4
int p_id[MAX_PATIENTS];
char p_name[MAX_PATIENTS][50];
int p_age[MAX_PATIENTS];
int p_urgency[MAX_PATIENTS];
int p_specialty_id[MAX_PATIENTS];
int p_is_admitted[MAX_PATIENTS];
int p_ward_id[MAX_PATIENTS];
double p_discount[MAX_PATIENTS];
int p_bed_num[MAX_PATIENTS];
int p_days_admitted[MAX_PATIENTS];
int patient_count=0;
double p_wait_time[MAX_PATIENTS];
double p_gross_total[MAX_PATIENTS];
double p_final_payable[MAX_PATIENTS];
int queue_counts[NUM_SPECIALTIES]={0,0,0,0};
int bedOccupancy[4][20]={0};
const char *SPECIALTY_NAMES[NUM_SPECIALTIES]={"General Practice(OPD)","Paediatrics","Cardiology","Neurology"};
const double BASE_FEES[NUM_SPECIALTIES]={1500.00,2500.00,4500.00,5000.00};
const char *WARD_NAMES[NUM_WARDS]={"General Ward","Paediatric Ward","Surgical Ward","ICU(Intensive Care Unit)"};
const double WARD_RATES[NUM_WARDS]={3000.00,6000.00,12000.00,25000.00};
const int CONSULTATION_TIMES[NUM_SPECIALTIES]={15,20,30,30};
const int WARD_CAPACITIES[NUM_WARDS]={20,10,10,5};
void mainmenue();
void register_patient();
void display_triage_queue();
void generate_analytics();
void display_bill(int idx);
void append_patient_file(int idx);
int allocate_bed(int ward_index,int*assigned_bed);
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
    printf("4.Exit System\n");
    printf("Enter Choice(1-4): ");
    scanf("%d",&choice);
    if(choice==1)
    { system("cls");
      register_patient();
    }
    else if(choice==2)
    {system("cls");
     display_triage_queue();
    }
    else if(choice==3)
    {system("cls");
      generate_analytics();
    }
    else if (choice==4)
    {
    printf("Exiting system...\n");
    }
    else
    {
     printf("Invalid option.Try again.\n");
    }


}
void register_patient()
{ if (patient_count>=MAX_PATIENTS)
    {
    printf("System capacity reached!\n");
    return;
    }
    int idx=patient_count;
    p_id[idx]=1001+idx;

    printf("\n---Patient Registration---\n");
    printf("Enter Patient Name:");
    getchar();
    fgets(p_name[idx],50,stdin);
    p_name[idx][strcspn(p_name[idx],"\n")]=0;

    printf("Enter Age:");
    scanf("%d",&p_age[idx]);

    printf("Enter Urgency Level (1=Normal,2=Urgent,3=Critical):");
    scanf("%d",&p_urgency[idx]);

    printf("\nSpecialties:\n");
    for (int i=0;i<NUM_SPECIALTIES;i++) {
        printf("%d.%s(Fee:LKR%.2f)\n",i+1,SPECIALTY_NAMES[i],BASE_FEES[i]);
    }
    printf("Select Specialty ID(1-4):");
    scanf("%d",&p_specialty_id[idx]);
    int spec_idx=p_specialty_id[idx]-1;

    printf("Is Patient Admitted to Ward? (1 = Yes, 0 = No): ");
    scanf("%d",&p_is_admitted[idx]);

    p_bed_num[idx]=0;
    if (p_is_admitted[idx]==1){
        printf("\nWards:\n");
        for (int i=0;i<NUM_WARDS;i++){
            printf("%d.%s(Rate:LKR%.2f/Day)\n",i+1,WARD_NAMES[i],WARD_RATES[i]);
        }
        printf("Select Ward ID (1-4):");
        scanf("%d",&p_ward_id[idx]);
        int w_idx=p_ward_id[idx]-1;

        if (!allocate_bed(w_idx,&p_bed_num[idx])) {
            printf("WARNING: Selected ward is at maximum capacity! Ward admission cancelled.\n");
            p_is_admitted[idx]=0;
            p_ward_id[idx]=0;
            p_days_admitted[idx]=0;
        } else {
            printf("Enter Days Admitted:");
            scanf("%d",&p_days_admitted[idx]);
        }
      }else {
        p_ward_id[idx]=0;
        p_days_admitted[idx]=0;
    }
      p_wait_time[idx]=(double)(queue_counts[spec_idx]*CONSULTATION_TIMES[spec_idx]);
    queue_counts[spec_idx]++;
    double base_fee=BASE_FEES[spec_idx];
    double surcharge=0.0;
    if (p_urgency[idx]==2)surcharge=base_fee*0.20;
    else if(p_urgency[idx]==3)surcharge=base_fee*0.50;

    double ward_cost=0.0;
    if (p_is_admitted[idx]==1){
        ward_cost=p_days_admitted[idx]*WARD_RATES[p_ward_id[idx]-1];
    }
    p_gross_total[idx]=base_fee+surcharge+ward_cost;

     if(p_age[idx]<5||p_age[idx]>65){
        p_discount[idx]=p_gross_total[idx]*0.15;
    }else{
        p_discount[idx]=0.0;
    }

    p_final_payable[idx]=p_gross_total[idx]-p_discount[idx];

    display_bill(idx);
    append_patient_file(idx);
    patient_count++;
}


void display_triage_queue()
{

}
void generate_analytics()
{

}
int allocate_bed(int ward_index,int*assigned_bed)
{

}
void display_bill(int idx)
{

}
void append_patient_file(int idx)
{

}


