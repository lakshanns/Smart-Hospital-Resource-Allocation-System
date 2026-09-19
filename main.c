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
char *SPECIALTY_NAMES[NUM_SPECIALTIES]={"General Practice(OPD)","Paediatrics","Cardiology","Neurology"};
double BASE_FEES[NUM_SPECIALTIES]={1500.00,2500.00,4500.00,5000.00};
char *WARD_NAMES[NUM_WARDS]={"General Ward","Paediatric Ward","Surgical Ward","ICU(Intensive Care Unit)"};
double WARD_RATES[NUM_WARDS]={3000.00,6000.00,12000.00,25000.00};
int CONSULTATION_TIMES[NUM_SPECIALTIES]={15,20,30,30};
int WARD_CAPACITIES[NUM_WARDS]={20,10,10,5};
void mainmenu();
void back_to_menu();
void register_patient();
void display_triage_queue();
void generate_analytics();
void display_bill(int idx);
int allocate_bed(int ward_index,int*assigned_bed);
int main()
{
    mainmenu();
    return 0;
}
void mainmenu()
{  int choice;
    printf("===================================================\n");
    printf("    SMART HOSPITAL MANAGEMENT SYSTEM - MENU\n");
    printf("===================================================\n");
    printf("1.Register New Patient Intake\n");
    printf("2.View Emergency Triage Priority Queue\n");
    printf("3.Display Analytics & Performance Report\n");
    printf("4.Exit System\n");
    printf("Enter Choice(1-4):");
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

    printf("Enter Urgency Level(1=Normal,2=Urgent,3=Critical):");
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
        printf("Select Ward ID(1-4):");
        scanf("%d",&p_ward_id[idx]);
        int w_idx=p_ward_id[idx]-1;

        if (!allocate_bed(w_idx,&p_bed_num[idx])) {
            printf("Selected ward is at maximum capacity! Ward admission cancelled.\n");
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
    system("cls");
    display_bill(idx);
    patient_count++;
    back_to_menu();
}
void display_triage_queue()
{ if (patient_count==0){
        printf("No patients currently registered in the queue\n");
        back_to_menu();
        return;
    }
    int order[MAX_PATIENTS];
    for (int i=0;i<patient_count;i++)order[i]=i;

    for (int i=0;i<patient_count-1;i++){
        int best_idx=i;
        for (int j=i+1;j<patient_count;j++){
            int u_curr=p_urgency[order[j]];
            int u_best=p_urgency[order[best_idx]];

            if (u_curr>u_best) {
                best_idx=j;
            } else if(u_curr==u_best){
                if(order[j]<order[best_idx]) {
                    best_idx=j;
                }
            }
        }
        int temp=order[i];
        order[i]=order[best_idx];
        order[best_idx]=temp;
    }

    printf("\n====================================================\n");
    printf("        EMERGENCY TRIAGE SORTED QUEUE               \n");
    printf("====================================================\n");
    printf("%s            %s        %s        %s\n","ID","Name","Urgency","Specialty");
    printf("----------------------------------------------------\n");
    for (int k=0;k<patient_count;k++){
        int idx=order[k];
        printf("PAT-%d    %s      Level-%d     %s\n",p_id[idx],p_name[idx],p_urgency[idx],SPECIALTY_NAMES[p_specialty_id[idx]-1]);
    }
    printf("====================================================\n");
    back_to_menu();
}


void generate_analytics()
{   printf("====================================================\n");
    printf("         PERFORMANCE REPORTS & ANALYTICS            \n");
    printf("====================================================\n");
    int lvl1=0,lvl2=0,lvl3=0;
    double total_revenue=0.0,total_discounts=0.0;
    int max_bill_idx=-1;
    double max_bill=-1.0;

    for (int i=0;i<patient_count;i++){
        if (p_urgency[i]==1)lvl1++;
        else if (p_urgency[i]==2)lvl2++;
        else if (p_urgency[i]==3)lvl3++;

        total_revenue+=p_final_payable[i];
        total_discounts+=p_discount[i];

        if (p_final_payable[i]>max_bill) {
            max_bill=p_final_payable[i];
            max_bill_idx=i;
        }
    }

    printf("1.Patient Category Breakdown:\n\n");
    printf("Total Patients    :%d\n",patient_count);
    printf("Level 1 (Normal)  :%d\n",lvl1);
    printf("Level 2 (Urgent)  :%d\n",lvl2);
    printf("Level 3 (Critical):%d\n\n",lvl3);

    printf("2.Financial Performance:\n\n");
    printf("Net Revenue Earned :LKR%.2f\n",total_revenue);
    printf("Total Discounts    :LKR%.2f\n\n",total_discounts);

    printf("3.Ward Occupancy Rate:\n\n");
    for (int w=0;w<NUM_WARDS;w++){
        int occupied=0;
        for (int b=0;b<WARD_CAPACITIES[w];b++) {
            if(bedOccupancy[w][b]==1)occupied++;
        }
        double percentage=((double)occupied/WARD_CAPACITIES[w])*100.0;
        printf("%s:%d/%d(%.1f%%)\n",WARD_NAMES[w],occupied,WARD_CAPACITIES[w],percentage);
    }

    printf("\n4.Highest Paying Patient Details:\n\n");
    if (max_bill_idx!=-1){
        printf("Name        :%s\n",p_name[max_bill_idx]);
        printf("Total Bill  :LKR %.2f\n",p_final_payable[max_bill_idx]);
    } else {
        printf("No patients processed yet.\n");
    }
    printf("====================================================\n");
    back_to_menu();
}


int allocate_bed(int ward_index,int*assigned_bed)
{
    int cap=WARD_CAPACITIES[ward_index];
    for (int j=0;j<cap;j++){
        if (bedOccupancy[ward_index][j]==0) {
            bedOccupancy[ward_index][j]=1;
            *assigned_bed=j+1;
            return 1;
        }
    }
    return 0;
}

void display_bill(int i)
{  int s_idx=p_specialty_id[i]-1;
    double base=BASE_FEES[s_idx];
    double surcharge=(p_urgency[i]==2)?base*0.20:((p_urgency[i]==3)?base*0.50:0.0);
    double ward_cost=(p_is_admitted[i]==1)?p_days_admitted[i]*WARD_RATES[p_ward_id[i]-1]:0.0;

    printf("\n====================================================\n");
    printf("         SMART HOSPITAL ADMISSION & BILL           \n");
    printf("----------------------------------------------------\n");
    printf("Patient ID     :PAT-%d\n",p_id[i]);
    printf("Patient Name   :%s\n",p_name[i]);
    printf("Age            :%d Years %s\n",p_age[i],(p_discount[i]>0)?"(15% Subsidy Eligible)":"");
    printf("Specialty      :%s\n",SPECIALTY_NAMES[s_idx]);
    if (p_is_admitted[i]==1) {
        printf("Assigned Ward  :%s(Bed %d)\n",WARD_NAMES[p_ward_id[i]-1],p_bed_num[i]);
    } else {
        printf("Assigned Ward  :None(Outpatient/OPD)\n");
    }
    printf("Urgency Level  :Leve l%d(%s)\n",p_urgency[i],
            (p_urgency[i]==3)?"Critical":((p_urgency[i]==2)?"Urgent":"Normal"));
    printf("----------------------------------------------------\n");
    printf("Base Consultation Fee :LKR %.2f\n",base);
    printf("Emergency Surcharge   :LKR %.2f(%s)\n",surcharge,(p_urgency[i]==3)?"50%":((p_urgency[i]==2)?"20%":"0%"));
    printf("Ward Stay Cost(%d Days) :LKR%.2f\n",p_days_admitted[i],ward_cost);
    printf("----------------------------------------------------\n");
    printf("Gross Total Bill      :LKR %.2f\n",p_gross_total[i]);
    printf("Age Subsidy Discount  :LKR %.2f (%s)\n",-p_discount[i],(p_discount[i]>0)?"15%":"0%");
    printf("----------------------------------------------------\n");
    printf("Final Payable Amount  :LKR %.2f\n",p_final_payable[i]);
    if (p_urgency[i]==3){
        printf("Estimated Waiting Time:0.00 mins(Immediate Attention)\n");
    } else {
        printf("Estimated Waiting Time:%.2f mins\n",p_wait_time[i]);
    }
    printf("====================================================\n");
}
void back_to_menu()
{    char choice;
     printf("\n\nBack to main menu(Y/N):");
                scanf(" %c",&choice);

                if (choice=='Y'||choice=='y')
                {
                    system("cls");
                    mainmenu();

                }
                else if(choice=='N'||choice=='n')
                {
                    system("cls");
                    printf("=========================================\n");
                    printf("          HAVE A NICE DAY!       \n");
                    printf("=========================================\n");

                }
                else
                {
                    printf("\n\tInvalid choice! Please enter Y or N.\n");
                }
}

