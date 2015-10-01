#-*- coding: utf8 -*-
import urllib
import urllib2
import re

url = 'http://lenta.ru'
name_txt = ''
name_txt = url[url.rfind('/') + 1 : url.rfind('.') + 1 ] + 'txt'
name_html = url[url.rfind('/') + 1 : url.rfind('.') + 1 ] + 'html'
urllib.urlretrieve(url, name_txt)
out_txt = 'output.txt'
content = urllib2.urlopen(url).read()
img_urls = re.findall('img .*?src="(.*?)"', content)
js_urls = re.findall('script .*?src="(http:*.*?)"', content)
css_urls = re.findall('href .*?src="(.css)"', content)
urls = img_urls + js_urls + css_urls


#print type(content)
#print content
#print type(urls)
print urls
