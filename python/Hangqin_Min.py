import My_Sql
from datetime import datetime, timedelta

class MyTime():
    def __init__(self):
        pass

def Open_file(path):
    fp = open(path,'r')
    lines = fp.readlines()
    return lines

def count_price(array):
    result = {}
    sum =  0
    result['High'] = 0
    result["low"] = 100000
    result["Open"] = 0
    result["close"] = 0
    result["Volume"] = 0
    result["ArgVolume"] = 0
    result["OpenInterest"] = 0
    for i in xrange(len(array)):
        pass



if __name__ == "__main__":
    dt_now = datetime.now() - timedelta(minutes = 5)
    dt_before = datetime.now() - timedelta(minutes = 6)
    now_day = dt_now.strftime('%Y%m%d')
    #now_time = dt_now.strftime('%H:%M:00')
    now_time = "14:56:00"
    before_day = dt_before.strftime('%Y%m%d')
    #before_time = dt_before.strftime('%H:%M:00')
    before_time = "14:55:00"
    InstrumentIDs = Open_file('/home/lizx/crawlled_page/test.txt')
    mysql_case = My_Sql.My_Sql()
    #sql_test = "select distinct `InstrumentID` from futuremarket"
    #print mysql_case.get_select_result(sql_test)
    for item in InstrumentIDs:
        sql = "SELECT * FROM futuremarket WHERE \
`InstrumentID`=\"%s\" and \
`TradingDay` = \"%s\" and `UpdateTime`> \"%s\" \
 and `UpdateTime`<=\"%s\""%(item, now_day,\
                    before_time, now_time) 
        qureyed_result = mysql_case.get_select_result(sql)
        print qureyed_result

SELECT * FROM `futuremarket` WHERE`InstrumentID`="al1510" and `TradingDay` = "20150320" and `UpdateTime`> "14:55:00" and `UpdateTime`<="14:56:00"