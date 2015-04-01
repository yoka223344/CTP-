__author__ = 'lizhongxiang'

import urllib2
import sys
import os
from datetime import date

class MySpider():
    def __init__(self):
        self.InstrumentID = {};

    def GetInstrument_SH(self, url):
        page_view = urllib2.urlopen(url).read()
        content = eval(page_view)
        for item in xrange(len(content["ContractDailyTradeArgument"])):
            self.InstrumentID[content["ContractDailyTradeArgument"][item]["INSTRUMENTID"]] \
            = "20150301"

    def GetInstrument_DL(self, url):
        page_view = urllib2.urlopen(url).read()
        lines = page_view.split("\n")
        for line in lines:
            if line.find( "contract") == -1:
                continue
            else:
                start = line.find("name=\"")
                end = line.find("\" ",start + 1)
                try:
                    self.InstrumentID[line[start + 6: end]] = "20150301"
                except Exception as e:
                    print(line)

    def GetInstrument_ZZ(self, url):
        page_view = urllib2.urlopen(url).read()
        lines = page_view.split("\n")
        for line in lines:
            if line.find(";\">") == -1:
                continue
            else:
                start = line.find(";\">")
                end = line.find("</a>",start + 1)
                try:
                    self.InstrumentID[line[start + 3: end]] = "20150301"
                except Exception as e:
                    print(line)
                    
    def write_doc(self, path = "./test.txt"):
        fp = open(path, 'w')
        for name in self.InstrumentID:
            line = name + "\n"
            fp.write(line)
        fp.close()


if __name__ == "__main__":
    today = date.today()
    today_str =  today.strftime("%Y%m%d")
    SH_url = "http://www.shfe.com.cn/data/instrument/ContractDailyTradeArgument%s.dat" % today_str
    DL_url = "http://quote.dce.com.cn/data/quoteAll.xml"
    ZZ_url = "http://www.czce.com.cn/portal/exchange/%s/quotation/%s.htm" %(today_str[:4], today_str)
    spider = MySpider()
    spider.GetInstrument_SH( SH_url)
    spider.GetInstrument_DL( DL_url)
    spider.GetInstrument_ZZ( ZZ_url)
    #store_path = "d:\\crawlled_page\\%s.txt" % today_str
    store_path = "/home/lizx/crawlled_page/test.txt"
    spider.write_doc(store_path)

