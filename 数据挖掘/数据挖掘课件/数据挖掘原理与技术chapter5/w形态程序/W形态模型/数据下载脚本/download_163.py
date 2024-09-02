#coding=utf8
import urllib2
import urllib
import os
import MakeURLs
import MakeStockCode

# 下载一个股票的所有数据
def download_data(stkcode):
    try:
        dir = os.path.dirname(os.path.realpath(__file__))
        dir += '/data_163/'
        url = 'http://quotes.money.163.com/service/chddata.html?code='+stkcode+'&start=19000000&end=99991230&fields=TOPEN;HIGH;LOW;TCLOSE;CHG;PCHG;TURNOVER;VOTURNOVER;VATURNOVER;TCAP;MCAP'
        urllib.urlretrieve(url, dir+stkcode+'.csv')
        return True
    except Exception,e:
        return False

# 下载所有股票的数据
def download_all(w_file_name):
    # 获取所有个股代码，保存在文件中
    MakeStockCode.get_stock_code(w_file_name)
    # 从文件中读取个股代码，r1是不带前缀的6位代码，r2包含前缀SH或SZ
    r1, r2 = MakeURLs.get_all_stkcode(w_file_name)
    for index, stkcode in enumerate(r1):
        # 只下载股票代码是0 3 6开头的A股数据
        if stkcode[0] in (['0','3','6']):
            # 上海股票代码前+0
            if r2[index][1]=='H':
                download_data('0'+stkcode)
            # 深圳股票代码前+1
            else:
                download_data('1' + stkcode)
            print '下载',stkcode

if __name__=="__main__":
    download_all('stkcode.txt')