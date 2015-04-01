import MySQLdb

def Get_right_Instrument():
    conn=MySQLdb.connect(host='localhost',\
            user='marketuser',passwd='abc0cba',db='MarketFlux',port=3306)
    cur=conn.cursor()
    cur.execute("select distinct `InstrumentID` from futuremarket")
    results = cur.fetchall()
    fp = open("/home/lizx/right_IN",'w')
    for item in results:
        fp.write(item[0] + '\n')
        #fp.write("\n")
    fp.close()
    cur.close()
    conn.close()

def Open_file(path):
    fp = open(path,'r')
    lines = fp.readlines()
    return lines

if __name__=="__main__":
    ri_lines = Open_file('/home/lizx/right_IN')
    test_lines = Open_file('/home/lizx/crawlled_page/test.txt')
    for test_line in test_lines:
        if test_line in ri_lines:
            continue
        else:
            print test_line

