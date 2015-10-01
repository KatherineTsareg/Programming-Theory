import urllib2
import re
import urllib


list_of_extensions = ['gif', 'bmp', 'jpg', 'jpeg', 'png']
url = 'http://lenta.ru'
c = urllib2.urlopen(url)
content = c.read()
img_urls = re.findall('img .*?src="(.*?)"', content)
for i in range(len(img_urls)):
    address = img_urls[i]
    if address[address.rfind('.') + 1 : ] in list_of_extensions:
        name_img = address[address.rfind('/') + 1 : ]
        urllib.urlretrieve(address, name_img)
