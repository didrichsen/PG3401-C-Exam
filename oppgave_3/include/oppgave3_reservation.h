#ifndef PG3401_V24_38_OPPGAVE3_RESERVATION_H
#define PG3401_V24_38_OPPGAVE3_RESERVATION_H

typedef struct _RESERVATION {

    struct _RESERVATION *pNextReservation;
    struct _RESERVATION *pPrevReservation;

    struct _ORDER *pOrder;

    int iTableNumber;
    int iNumberOfSeats;
    int iHour;
    int iMinute;
    int iDay;
    int iMonth;
    int iYear;
    char acGuestName[];

}RESERVATION;

typedef struct _RESERVATION_LIST{

    struct _RESERVATION *pHead;
    struct _RESERVATION *pTail;

} RESERVATION_LIST;

int ValidNumber(int iInput, int iRangeStart,int iRangeEnd);
int InsertReservation(RESERVATION_LIST *pList, int iTableNumber, int iNumberOfSeats,int iDay,int iMonth,int iYear, int iHour, int iMinutes,char *acGuestName);
int DeleteReservationBasedOnGuestName(RESERVATION_LIST *pList, char *pszGuestName);
RESERVATION *FindReservation(RESERVATION_LIST *pList, int iTableNumber);
void ListReservationByTableNumber(RESERVATION_LIST *pList, int tableNumber);
int DeleteReservations(RESERVATION_LIST *pList);
RESERVATION *FindReservationByIndex(RESERVATION_LIST *pList, int iReservationNumberOrdered);
void PrintReservationAndOrders(RESERVATION *pReservation);
RESERVATION *FindReservationByName(RESERVATION_LIST *pList, char *acGuestName);
void PrintReservationDetails(RESERVATION *pReservation);
int DeleteReservations(RESERVATION_LIST *pList);
void PrintReceipt(RESERVATION *pReservation, int iPayTogether);

#endif //PG3401_V24_38_OPPGAVE3_RESERVATION_H