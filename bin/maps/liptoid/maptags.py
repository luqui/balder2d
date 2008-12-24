from scipy import *

print '<?xml version="1.0"?>'
print '<!-- information about the map -->'
print '<Map name="liptoid">'
print '\t<Size width="1024" height="748" />'
print '\t<!-- Spawn Locations -->'
for a in range(0,8):
     x=120*cos(a*pi/4)+512
     y=120*sin(a*pi/4)+384
     print '\t<spawn>'
     print '\t\t<location x="',x,'" y="',y,'" />'
     xv = 3*cos(a*pi/4)
     yv = 3*sin(a*pi/4)
     print '\t\t<velocity x="',xv,'" y="',yv,'" />'
     print '\t\t<stuck status="false" />'
     print '\t</spawn>'

print '</Map>'
