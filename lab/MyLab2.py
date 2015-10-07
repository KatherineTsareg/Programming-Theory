import urllib
import urllib2
import re


list_of_extensions = ['gif', 'bmp', 'jpg', 'jpeg', 'png', 'js', 'css']


url = 'http://lenta.ru'
content = urllib2.urlopen(url).read()
img_urls = re.findall('img.*?src="(.*?)"', content)
js_urls = re.findall('script.*?src="(http:*.*?)"', content)
css_urls = re.findall('link.*?href=\"(.*?.css)\"',content)
urls = img_urls + js_urls + css_urls
print urls
for i in range(len(urls)):
    address = urls[i]
    if address[address.rfind('.') + 1 : ] in list_of_extensions:
        name_file = address[address.rfind('/') + 1 : ]
        urllib.urlretrieve(address, name_file)
print 'end'
