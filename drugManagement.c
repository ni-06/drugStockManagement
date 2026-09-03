#include <stdio.h>


//Database
int stockInfo[DRUG_COUNT][TOTAL_COLS];
char drugNames[DRUG_COUNT][30];

//Global Constant
#define DRUG_COUNT 6
#define TOTAL_COLS 9

//array contant
#define ID_COL 0
#define QTY_COL 1
#define DAILY_USE 2
#define MIN_LEVEL 3
#define EXPIRE_DAYS 4
#define IMPORTANCE 5
#define DAYS_LEFT 6
#define STATUS 7
#define RANKING 8

// Stock status codes
#define STATUS_OK 1
#define STATUS_REORDER 2
#define STATUS_URGENT 3
#define STATUS_EXPIRING 4
#define STATUS_CRITICAL 5

// Risk thresholds
#define LOW_COVERAGE 5
#define EXPIRE_SOON 20

//array contant
#define ID_COL 0
#define QTY_COL 1
#define DAILY_USE 2
#define MIN_LEVEL 3
#define EXPIRE_DAYS 4
#define IMPORTANCE 5
#define DAYS_LEFT 6
#define STATUS 7
#define RANKING 8

// Stock status codes
#define STATUS_OK 1
#define STATUS_REORDER 2
#define STATUS_URGENT 3
#define STATUS_EXPIRING 4
#define STATUS_CRITICAL 5

// Risk thresholds
#define LOW_COVERAGE 5
#define EXPIRE_SOON 20

//array contant
#define ID_COL 0
#define QTY_COL 1
#define DAILY_USE 2
#define MIN_LEVEL 3
#define EXPIRE_DAYS 4
#define IMPORTANCE 5
#define DAYS_LEFT 6
#define STATUS 7
#define RANKING 8

// Stock status codes
#define STATUS_OK 1
#define STATUS_REORDER 2
#define STATUS_URGENT 3
#define STATUS_EXPIRING 4
#define STATUS_CRITICAL 5

// Risk thresholds
#define LOW_COVERAGE 5
#define EXPIRE_SOON 20

// FUNCTION DECLARATIONS
void showMainMenu(void);
void getDrugData(void);
void getDrugNames(void);
void showRawData(void);
int daysOfSupply(int available, int dailyNeed);
void updateAllSupplyDays(void);
int evaluateSituation(int stock, int minimum, int expiry, int essential, int supplyDays);
void evaluateAllMedicines(void);
void displayStatus(int condition);
void generateFullReport(void);
int findDrug(int targetCode, int position);
void searchMedicine(void);
void adjustStock(void);
int urgencyScore(int index);
int compareUrgency(int item1, int item2);
void computeAllRanks(void);
void exchangeRows(int a, int b);
void arrangeByPriority(void);
void showPriorityList(void);


// MAIN PROGRAM

int main()
{
    int userChoice;

    printf("\n\n");
    printf("  MEDICINE INVENTORY CONTROL SYSTEM\n");
    printf("  Community Health Centre\n");

    do
    {
        showMainMenu();
        printf("  Enter selection: ");
        scanf("%d", &userChoice);

        switch(userChoice)
        {
            case 1:
                getDrugData();
                break;
            case 2:
                getDrugNames();
                break;
            case 3:
                showRawData();
                break;
            case 4:
                updateAllSupplyDays();
                evaluateAllMedicines();
                generateFullReport();
                break;
            case 5:
                searchMedicine();
                break;
            case 6:
                adjustStock();
                break;
            case 7:
                updateAllSupplyDays();
                evaluateAllMedicines();
                computeAllRanks();
                arrangeByPriority();
                showPriorityList();
                break;
            case 8:
                printf("\n  Thank you for using the system!\n");
                break;
            default:
                printf("\n  Invalid option! Please choose 1-8.\n");
        }

    } while(userChoice != 8);

    return 0;
}
void getDrugData(void)
{
    int i;
    printf("\nEnter Drugs Information\n");
    printf("  Format: Code Stock DailyNeed MinStock Expiry Importance(1-3)\n");
    printf("  Example: 1 120 35 80 45 3\n\n");

    for(int i=0; i<DRUG_COUNT; i++)
    {
        scanf("%d %d %d %d %d %d",
              &stockInfo[i][ID_COL],
              &stockInfo[i][QTY_COL],
              &stockInfo[i][DAILY_USE],
              &stockInfo[i][MIN_LEVEL],
              &stockInfo[i][EXPIRE_DAYS],
              &stockInfo[i][IMPORTANCE]);

        stockInfo[i][DAYS_LEFT] = 0;
        stockInfo[i][STATUS] = 0;
        stockInfo[i][RANKING] = 0;
    }

    printf("\n  Data stored successfully!\n");
}

void getDrugNames(void)
{
    printf("\n  --- Enter Drug Names ---\n");

    for(int i = 0; i < DRUG_COUNT; i++)
    {
        printf("  Name for drug %d: ", i + 1);
        scanf("%s", drugNames[i]);
    }

    printf("\n  Names saved successfully!\n");
}

void showRawData(void)
{
    printf("\n\n");
    printf("  =========================================\n");
    printf("  CURRENT INVENTORY DATA\n");
    printf("  =========================================\n");
    printf("  Code  Name            Stock  Daily  Min   Expiry  Imp\n");
    printf("  ------------------------------------------------------\n");

    for(int i = 0; i < DRUG_COUNT; i++)
    {
        printf("  %d     %-15s %d     %d     %d     %d      %d\n",
               stockInfo[i][ID_COL],
               drugNames[i],
               stockInfo[i][QTY_COL],
               stockInfo[i][DAILY_USE],
               stockInfo[i][MIN_LEVEL],
               stockInfo[i][EXPIRE_DAYS],
               stockInfo[i][IMPORTANCE]);
    }
}

// COVERAGE CALCULATION
//  
int daysOfSupply(int available, int dailyNeed)
{
    if(dailyNeed <= 0)
    {
        if(available == 0)
            return 0;
        else
            return 999;
    }
    int r=available / dailyNeed;
    return r;
}
void calculateDaysOfSupply(void)
{
    for(int i = 0; i < DRUG_COUNT; i++)
    {
        stockInfo[i][DAYS_LEFT] = daysOfSupply(
            stockInfo[i][QTY_COL],
            stockInfo[i][DAILY_USE]
        );
    }
}

// STATUS EVALUATION
int evaluateSituation(int stock, int minimum, int expiry, int essential, int supplyDays)
{
    int belowMinimum = (stock < minimum);
    int nearExpiry = (expiry <= EXPIRE_SOON);
    int supplyLow = (supplyDays <= LOW_COVERAGE);

    // Decision hierarchy
    if(belowMinimum && nearExpiry)
        return STATUS_CRITICAL;

    else if(belowMinimum && essential == 3)
        return STATUS_URGENT;

    else if(belowMinimum)
        return STATUS_REORDER;

    else if(essential == 3 && supplyLow)
        return STATUS_URGENT;

    else if(nearExpiry)
        return STATUS_EXPIRING;

    else
        return STATUS_OK;
}

void evaluateAllMedicines(void)
{
    for(int i = 0; i < DRUG_COUNT; i++)
    {
        stockInfo[i][STATUS] = evaluateSituation(
            stockInfo[i][QTY_COL],
            stockInfo[i][MIN_LEVEL],
            stockInfo[i][EXPIRE_DAYS],
            stockInfo[i][IMPORTANCE],
            stockInfo[i][DAYS_LEFT]
        );
    }
}

//  
// STATUS TEXT DISPLAY
//  
void displayStatus(int condition)
{
    switch(condition)
    {
        case STATUS_OK:
            printf("Adequate Stock");
            break;
        case STATUS_REORDER:
            printf("Order Required");
            break;
        case STATUS_URGENT:
            printf("URGENT Order");
            break;
        case STATUS_EXPIRING:
            printf("Expiry Alert");
            break;
        case STATUS_CRITICAL:
            printf("CRITICAL Situation");
            break;
        default:
            printf("Unknown Status");
    }
}

// FULL REPORT GENERATION
void generateFullReport(void)
{
    printf("\n\n");
    printf("  COMPLETE INVENTORY REPORT\n");
    printf("\n");
    printf("  Code  Name            Stock  Coverage  Expiry  Imp  Status\n");

    for(int i = 0; i < DRUG_COUNT; i++)
    {
        printf("  %d     %-15s %d     %d days    %d      %d    ",
               stockInfo[i][ID_COL],
               drugNames[i],
               stockInfo[i][QTY_COL],
               stockInfo[i][DAYS_LEFT],
               stockInfo[i][EXPIRE_DAYS],
               stockInfo[i][IMPORTANCE]);

        displayStatus(stockInfo[i][STATUS]);
        printf("\n");
    }
    printf("\n");
}
void findDrug(int targetCode, int position)
{
    for(int i = 0; i < DRUG_COUNT; i++)
    {
        if(stockInfo[i][ID_COL] == targetCode)
        {
            position = i;
            return;
        }
    }
    position = -1; // Not found
}
void searchMedicine(void)
{
    int code, position;
    printf("\n  Enter drug code to search: ");
    scanf("%d", &code);

    findDrug(code, position);

    if(position != -1)
    {
        printf("  Drug found: %s\n", drugNames[position]);
        printf("  Stock: %d\n", stockInfo[position][QTY_COL]);
        printf("  Daily Use: %d\n", stockInfo[position][DAILY_USE]);
        printf("  Minimum Level: %d\n", stockInfo[position][MIN_LEVEL]);
        printf("  Expiry Days: %d\n", stockInfo[position][EXPIRE_DAYS]);
        printf("  Importance: %d\n", stockInfo[position][IMPORTANCE]);
        printf("  Days of Supply Left: %d\n", stockInfo[position][DAYS_LEFT]);
        printf("  Status: ");
        displayStatus(stockInfo[position][STATUS]);
        printf("\n");
    }
    else
    {
        printf("  Drug with code %d not found.\n", code);
    }
}
//stock adjustment
void adjustStock(void)
{
    int code, position, newStock;
    printf("\n  Enter drug code to adjust stock: ");
    scanf("%d", &code);

    findDrug(code, position);

    if(position != -1)
    {
        printf("  Current stock for %s: %d\n", drugNames[position], stockInfo[position][QTY_COL]);
        printf("  Enter new stock quantity: ");
        scanf("%d", &newStock);
        stockInfo[position][QTY_COL] = newStock;
        printf("  Stock updated successfully!\n");
    }
    else
    {
        printf("  Drug with code %d not found.\n", code);
    }
}

//priority ranking
int urgencyScore(int index)
{
    int score = 0;
    if(stockInfo[index][STATUS] == STATUS_CRITICAL)
        score += 5;
    else if(stockInfo[index][STATUS] == STATUS_URGENT)
        score += 4;
    else if(stockInfo[index][STATUS] == STATUS_REORDER)
        score += 3;
    else if(stockInfo[index][STATUS] == STATUS_EXPIRING)
        score += 2;
    else
        score += 1;

    // Add importance to the score
    score += stockInfo[index][IMPORTANCE];

    return score;
}
