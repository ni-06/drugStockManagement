#include <stdio.h>

#define DRUG_COUNT 6
#define TOTAL_COLS 9

// Column positions in stockInfo array
#define ID_COL 0
#define QTY_COL 1
#define DAILY_USE 2
#define MIN_LEVEL 3
#define EXPIRE_DAYS 4
#define IMPORTANCE 5
#define DAYS_LEFT 6
#define STATUS 7
#define RANKING 8

// Stock status
#define STATUS_OK 1
#define STATUS_REORDER 2
#define STATUS_URGENT 3
#define STATUS_EXPIRING 4
#define STATUS_CRITICAL 5


#define LOW_COVERAGE 5
#define EXPIRE_SOON 20

//  DRUG DATA STORAGE
int stockInfo[DRUG_COUNT][TOTAL_COLS];
char drugNames[DRUG_COUNT][30];

//  FUNCTION DECLARATIONS 
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
    printf("   \n");
    printf("  MEDICINE INVENTORY CONTROL SYSTEM\n");
    printf("  Community Health Centre\n");
    printf("   \n");

    do
    {
        showMainMenu();
        printf("  Enter your selection: ");
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
                printf("   \n\n");
                break;
            default:
                printf("\n  Invalid option! Please choose 1-8.\n");
        }

    } while(userChoice != 8);

    return 0;
}

 
// MENU DISPLAY
 
void showMainMenu(void)
{
    printf("\n\n");
    printf("   \n");
    printf("  MAIN MENU\n");
    printf("   \n");
    printf("  1. Enter Medicine Data\n");
    printf("  2. Enter Medicine Names\n");
    printf("  3. View All Data\n");
    printf("  4. Generate Full Status Report\n");
    printf("  5. Find Medicine by Code\n");
    printf("  6. Update Stock Quantity\n");
    printf("  7. Show Priority Order\n");
    printf("  8. Exit Program\n");
    printf("   \n");
}

 
// INPUT FUNCTIONS
 
void getDrugData(void)
{
    printf("\n  --- Enter Medicine Information ---\n");
    printf("  Format: Code Stock DailyNeed MinStock Expiry Importance(1-3)\n");
    printf("  Example: 1 120 35 80 45 3\n\n");

    for(int i = 0; i < DRUG_COUNT; i++)
    {
        printf("  Drug %d: ", i + 1);
        scanf("%d %d %d %d %d %d",
              &stockInfo[i][ID_COL],
              &stockInfo[i][QTY_COL],
              &stockInfo[i][DAILY_USE],
              &stockInfo[i][MIN_LEVEL],
              &stockInfo[i][EXPIRE_DAYS],
              &stockInfo[i][IMPORTANCE]);

        // Reset calculated fields
        stockInfo[i][DAYS_LEFT] = 0;
        stockInfo[i][STATUS] = 0;
        stockInfo[i][RANKING] = 0;
    }

    printf("\n  Data stored successfully!\n");
}

void getDrugNames(void)
{
    printf("Enter Drug Names\n");

    for(int i = 0; i < DRUG_COUNT; i++)
    {
        printf("Name for drug %d: ", i+1);
        scanf("%s", drugNames[i]);
    }

    printf("\nNames saved successfully!\n");
}

 
// DISPLAY FUNCTIONS
 
void showRawData(void)
{
    printf("\n\n");
    printf("   \n");
    printf("  CURRENT INVENTORY DATA\n");
    printf("   \n");
    printf("  Code  Name            Stock  Daily  Min   Expiry  Imp\n");
    printf("  \n");

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
 
int daysOfSupply(int available, int dailyNeed)
{
    if(dailyNeed<=0)
    {
        if(available==0)
            return 0;
        else
            return 999;
    }
    return available / dailyNeed;
}

void updateAllSupplyDays(void)
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
    int belowMinimum = (stock< minimum);
    int nearExpiry = (expiry <=EXPIRE_SOON);
    int supplyLow = (supplyDays <=LOW_COVERAGE);

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
    for(int i=0; i <DRUG_COUNT; i++)
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

 
// STATUS TEXT DISPLAY
 
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
    printf("\n");
    printf("  COMPLETE INVENTORY REPORT\n");
    printf("   \n");
    printf("  Code  Name            Stock  Coverage  Expiry  Imp  Status\n");
    printf("\n");

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
    printf("   \n");
}

 
// SEARCH FUNCTION (RECURSIVE)
 
int findDrug(int targetCode, int position)
{
    // Base case 1: End of array
    if(position>= DRUG_COUNT)
        return -1;

    // Base case 2: Found match
    if(stockInfo[position][ID_COL] == targetCode)
        return position;

    // Recursive case
    return findDrug(targetCode, position + 1);
}

void searchMedicine(void)
{
    int targetCode;
    printf("\n  Enter drug code to find: ");
    scanf("%d", &targetCode);

    int index = findDrug(targetCode, 0);

    if(index == -1)
    {
        printf("\n  Drug with code %d does not exist!\n", targetCode);
    }
    else
    {
        printf("\n\n");
        printf("   \n");
        printf("  DRUG FOUND\n");
        printf("   \n");
        printf("  Code: %d\n", stockInfo[index][ID_COL]);
        printf("  Name: %s\n", drugNames[index]);
        printf("  Quantity in Stock: %d\n", stockInfo[index][QTY_COL]);
        printf("  Daily Consumption Rate: %d\n", stockInfo[index][DAILY_USE]);
        printf("  Minimum Stock Level: %d\n", stockInfo[index][MIN_LEVEL]);
        printf("  Days Until Expiry: %d\n", stockInfo[index][EXPIRE_DAYS]);
        printf("  Essentiality Rating: %d\n", stockInfo[index][IMPORTANCE]);
        printf("  Days of Supply Left: %d\n", stockInfo[index][DAYS_LEFT]);
        printf("  Current Status: ");
        displayStatus(stockInfo[index][STATUS]);
        printf("\n   \n");
    }
}

 
// STOCK ADJUSTMENT
 
void adjustStock(void)
{
    int targetCode, operation, amount;

    printf("\n  Enter drug code to adjust: ");
    scanf("%d", &targetCode);

    int index= findDrug(targetCode, 0);

    if(index == -1)
    {
        printf("  Drug not found!\n");
        return;
    }

    printf("\n\n");
    printf("   \n");
    printf("  STOCK ADJUSTMENT\n");
    printf("   \n");
    printf("  Drug: %s (Code: %d)\n", drugNames[index], stockInfo[index][ID_COL]);
    printf("  Current Quantity: %d\n", stockInfo[index][QTY_COL]);
    printf("\n  1. Receive New Stock\n");
    printf("  2. Dispense Stock to Patients\n");
    printf("  Enter option: ");
    scanf("%d", &operation);

    printf("  Enter quantity: ");
    scanf("%d", &amount);

    if(amount < 0)
    {
        printf("  Quantity cannot be negative!\n");
        return;
    }

    if(operation==1)
    {
        stockInfo[index][QTY_COL] += amount;
        printf("\n  Received %d units. New stock: %d\n", amount, stockInfo[index][QTY_COL]);
    }
    else if(operation ==2)
    {
        if(amount > stockInfo[index][QTY_COL])
        {
            printf("\n  Only %d units available. Dispensing all remaining.\n",
                   stockInfo[index][QTY_COL]);
            stockInfo[index][QTY_COL] = 0;
        }
        else
        {
            stockInfo[index][QTY_COL] -= amount;
            printf("\n  Dispensed %d units. Remaining: %d\n",
                   amount, stockInfo[index][QTY_COL]);
        }
    }
    else
    {
        printf("\n  Invalid operation!\n");
        return;
    }

    // Update calculations for this medicine only
    stockInfo[index][DAYS_LEFT] = daysOfSupply(
        stockInfo[index][QTY_COL],
        stockInfo[index][DAILY_USE]
    );

    stockInfo[index][STATUS] = evaluateSituation(
        stockInfo[index][QTY_COL],
        stockInfo[index][MIN_LEVEL],
        stockInfo[index][EXPIRE_DAYS],
        stockInfo[index][IMPORTANCE],
        stockInfo[index][DAYS_LEFT]
    );

    printf("  Stock status updated!\n");
    printf("   \n");
}

 
// PRIORITY CALCULATION

int urgencyScore(int index)
{
    int condition = stockInfo[index][STATUS];
    int importance = stockInfo[index][IMPORTANCE];
    int supplyDays = stockInfo[index][DAYS_LEFT];

    // Base value based on condition
    int baseScore;
    if(condition == STATUS_CRITICAL)
        baseScore = 100;
    else if(condition == STATUS_URGENT)
        baseScore = 80;
    else if(condition == STATUS_REORDER)
        baseScore = 60;
    else if(condition == STATUS_EXPIRING)
        baseScore = 40;
    else
        baseScore = 0;

    return baseScore + (importance * 10) - supplyDays;
}

int compareUrgency(int item1, int item2)
{
    int score1 = urgencyScore(item1);
    int score2 = urgencyScore(item2);

    if(score1 != score2)
        return score1 > score2;

    // Tie-breakers
    int imp1 = stockInfo[item1][IMPORTANCE];
    int imp2 = stockInfo[item2][IMPORTANCE];
    if(imp1 != imp2)
        return imp1 > imp2;

    int exp1 = stockInfo[item1][EXPIRE_DAYS];
    int exp2 = stockInfo[item2][EXPIRE_DAYS];
    if(exp1 != exp2)
        return exp1 < exp2;

    return stockInfo[item1][ID_COL] < stockInfo[item2][ID_COL];
}

void computeAllRanks(void)
{
    for(int i = 0; i < DRUG_COUNT; i++)
    {
        int rank = 1;
        for(int j = 0; j < DRUG_COUNT; j++)
        {
            if(j != i && compareUrgency(j, i))
                rank++;
        }
        stockInfo[i][RANKING] = rank;
    }
}

// SORTING

void exchangeRows(int a, int b)
{
    // Swap numeric data
    for(int col = 0; col < TOTAL_COLS; col++)
    {
        int temp = stockInfo[a][col];
        stockInfo[a][col] = stockInfo[b][col];
        stockInfo[b][col] = temp;
    }

    // Swap names
    for(int ch = 0; ch < 30; ch++)
    {
        char tempChar = drugNames[a][ch];
        drugNames[a][ch] = drugNames[b][ch];
        drugNames[b][ch] = tempChar;
    }
}

void arrangeByPriority(void)
{
    for(int i = 0; i < DRUG_COUNT - 1; i++)
    {
        int smallestPos = i;

        for(int j = i + 1; j < DRUG_COUNT; j++)
        {
            if(stockInfo[j][RANKING] < stockInfo[smallestPos][RANKING])
                smallestPos = j;
        }

        if(smallestPos != i)
            exchangeRows(i, smallestPos);
    }
}

// PRIORITY REPORT

void showPriorityList(void)
{
    printf("\n\n");
    printf("\n");
    printf("  PRIORITY ORDER (1=Most Urgent)\n");
    printf("\n");
    printf("  Rank  Code  Name            Stock  Coverage  Status\n");
    printf("\n");

    for(int i=0; i<DRUG_COUNT; i++)
    {
        printf("  %d     %d     %-15s %d     %d days    ",
               stockInfo[i][RANKING],
               stockInfo[i][ID_COL],
               drugNames[i],
               stockInfo[i][QTY_COL],
               stockInfo[i][DAYS_LEFT]);

        displayStatus(stockInfo[i][STATUS]);
        printf("\n");
    }
    printf("\n");
}