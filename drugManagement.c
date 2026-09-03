#include <stdio.h>

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

//GLOBAL VARIABLES
int stockInfo[DRUG_COUNT][TOTAL_COLS];
char drugNames[DRUG_COUNT][30];

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
    
}