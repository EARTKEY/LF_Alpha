# LF_Alpha - Third-Party Software Notices

This document contains the copyright notices and license information for third-party software components included in the LF_Alpha library.

---

## Summary

The LF_Alpha library includes code from the following open-source projects:

| Component | Copyright Holder | License | Source |
|-----------|-----------------|---------|--------|
| MPU6050 / I2Cdev | Jeff Rowberg, ElectronicCats | MIT | https://github.com/ElectronicsCats/mpu6050 |
| Adafruit_SSD1306 | Adafruit Industries | BSD | https://github.com/adafruit/Adafruit_SSD1306 |
| Adafruit_NeoPixel | Adafruit Industries | LGPL-3.0 | https://github.com/adafruit/Adafruit_NeoPixel |
| Adafruit_APDS9960 | Adafruit Industries | MIT | https://github.com/adafruit/Adafruit_APDS9960 |
| AHT20 | dvarrel | CC0-1.0 | https://github.com/dvarrel/AHT20 |
| DS3231-RTC | Frank Häfele (hasenradball) | MIT | https://github.com/hasenradball/DS3231-RTC |
| DFRobot_MAX30102 | DFRobot | MIT | https://github.com/DFRobot/DFRobot_MAX30102 |
| ESP32Servo | Kevin Harrington | LGPL-2.1 | https://github.com/madhephaestus/ESP32Servo |

**Complete license texts are available in each component's directory under `src/lib/[ComponentName]/LICENSE.txt`**

---

## Compliance Notes

### LGPL Components

This project includes two components licensed under the LGPL (Lesser General Public License):
- **Adafruit_NeoPixel** (LGPL-3.0)
- **ESP32Servo** (LGPL-2.1)

**What this means:**
- Source code for these components is available at their respective GitHub repositories
- Users are free to replace these components with modified versions
- If you distribute binaries containing this library, you must provide access to the source code of these LGPL components

**Compliance:** All LGPL source code is included in this repository under `src/lib/` with full license files.

---

## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

---

**Last Updated:** October 28, 2025  
**LF_Alpha Version:** 1.0.0
