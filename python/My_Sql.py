import MySQLdb

class My_Sql():
    def __init__(self, host = "localhost" , 
            user = "marketuser", passwd = "abc0cba", 
            db = "MarketFlux", port = 3306):
        self._conn = None
        self._cursor = None
        self._result = None
        self.get_connect(host, user, passwd, db, port)

    def execute_sql(self, sql):
        n =  self._cursor.execute(sql)
        if n == 0:
            print "database has not changed %s" % sql
        else:
            print "sql execute success, %s line(s) affect" % n
        return n

    def get_select_result(self, sql):
        self._cursor.execute(sql)
        self._result = self._cursor.fetchall()
        return self._result

    def get_connect(self, host, user, passwd, db, port):
        self._conn = MySQLdb.connect(host = host,
                user = user, passwd = passwd, db = db, port = port)
        self._cursor = self._conn.cursor()
    
    def close_connect(self):
        self._cursor.close()
        self._conn.close()


