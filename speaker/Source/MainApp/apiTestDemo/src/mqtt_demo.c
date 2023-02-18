#include "MercuryPlat.h"
#include "MercuryMem.h"
#include "MercuryDef.h"
#include "md5.h"
#include "cjson.h"
#include "mqttclient.h"

//������֤ʱ�ķ�����
#define SERVER_REGISTER_URL "https://iothub-bank.inspos.cn/auth/register/device"
#define SERVER_REGISTER_HOST "iothub-bank.inspos.cn"

// #define MQTT_SERVER_URL "mqtt-bank.inspos.cn"
#define MQTT_SERVER_URL "test.mosquitto.org"

#define SERVER_CA_CRT   \
"-----BEGIN CERTIFICATE-----\r\n" \
"MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh\r\n" \
"MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\r\n" \
"d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\r\n" \
"QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT\r\n" \
"MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\r\n" \
"b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG\r\n" \
"9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB\r\n" \
"CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97\r\n" \
"nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt\r\n" \
"43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P\r\n" \
"T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4\r\n" \
"gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO\r\n" \
"BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR\r\n" \
"TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw\r\n" \
"DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr\r\n" \
"hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg\r\n" \
"06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF\r\n" \
"PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls\r\n" \
"YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk\r\n" \
"CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=\r\n" \
"-----END CERTIFICATE-----\r\n" 

#define MQTT_KEY_CRT  \
"-----BEGIN CERTIFICATE-----\r\n" \
"MIIGAzCCBOugAwIBAgIQBU+QVhwN7Z8r+BB4v1xkqzANBgkqhkiG9w0BAQsFADBu\r\n" \
"MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\r\n" \
"d3cuZGlnaWNlcnQuY29tMS0wKwYDVQQDEyRFbmNyeXB0aW9uIEV2ZXJ5d2hlcmUg\r\n" \
"RFYgVExTIENBIC0gRzEwHhcNMjEwNDA4MDAwMDAwWhcNMjIwNDA4MjM1OTU5WjAh\r\n" \
"MR8wHQYDVQQDExZtcXR0LWNsaWVudDEuaW5zcG9zLmNuMIIBIjANBgkqhkiG9w0B\r\n" \
"AQEFAAOCAQ8AMIIBCgKCAQEAuH221NBjVKf56TqcNeDMxldarerckCuuOXa1fhjN\r\n" \
"ENOCK1BhUdHFyiUHawVnJQ7UvB31fgVHlHDejLfHDZpBbtkzOTeaixSFeZDCggpU\r\n" \
"1CDvdnzRTDhwcUFnyS+fPc3jSXD6EtNxIlDcyvQHhoIRekepZn2ipxzWXHArIr2B\r\n" \
"WvLyH1CmSyup1O3VZJA35+ff4vX8l7uH5o7TIWWSVkKrGW3ONMMs2WSMgCSjamIt\r\n" \
"U4VGA1q+sZRYRsTR0EnbooakufcnStVE75/27WF0Vt7D3MUy4xYuoWDgF79uLFcW\r\n" \
"ywCATO+Gy6wWA5gM/oyMans99slL4TzsRGYpJNY/ARezoQIDAQABo4IC6DCCAuQw\r\n" \
"HwYDVR0jBBgwFoAUVXRPsnJP9WC6UNHX5lFcmgGHGtcwHQYDVR0OBBYEFMw14biT\r\n" \
"wGdcmnwXOcVk/lrP31IqMCEGA1UdEQQaMBiCFm1xdHQtY2xpZW50MS5pbnNwb3Mu\r\n" \
"Y24wDgYDVR0PAQH/BAQDAgWgMB0GA1UdJQQWMBQGCCsGAQUFBwMBBggrBgEFBQcD\r\n" \
"AjA+BgNVHSAENzA1MDMGBmeBDAECATApMCcGCCsGAQUFBwIBFhtodHRwOi8vd3d3\r\n" \
"LmRpZ2ljZXJ0LmNvbS9DUFMwgYAGCCsGAQUFBwEBBHQwcjAkBggrBgEFBQcwAYYY\r\n" \
"aHR0cDovL29jc3AuZGlnaWNlcnQuY29tMEoGCCsGAQUFBzAChj5odHRwOi8vY2Fj\r\n" \
"ZXJ0cy5kaWdpY2VydC5jb20vRW5jcnlwdGlvbkV2ZXJ5d2hlcmVEVlRMU0NBLUcx\r\n" \
"LmNydDAJBgNVHRMEAjAAMIIBgAYKKwYBBAHWeQIEAgSCAXAEggFsAWoAdgBGpVXr\r\n" \
"dfqRIDC1oolp9PN9ESxBdL79SbiFq/L8cP5tRwAAAXiw8kNdAAAEAwBHMEUCIEH2\r\n" \
"66LmT/CEqoKr3Q9nTpmasYQChz/pCRrqYCqCy9hZAiEA2fQtsKO/n+AQiENhYfu0\r\n" \
"PYXNX3iwAkKzzVZmU7yrymwAdwAiRUUHWVUkVpY/oS/x922G4CMmY63AS39dxoNc\r\n" \
"buIPAgAAAXiw8kM4AAAEAwBIMEYCIQCYilkhGHWf6lrdLAIq8D2+lDvr7vn70zb0\r\n" \
"xDmKNnI86gIhANsN6u8ebAnGi67fOh4v49q64GQR+ThQbbycau5QFPYfAHcAUaOw\r\n" \
"9f0BeZxWbbg3eI8MpHrMGyfL956IQpoN/tSLBeUAAAF4sPJDUQAABAMASDBGAiEA\r\n" \
"2HSLybCV2XApvDVaRPiNviWHC2L/WcJRVzVjManX++kCIQC5tnsYPyFyNAWfUW5E\r\n" \
"bU51WD20TWNfqqzlGpRqWp4WxjANBgkqhkiG9w0BAQsFAAOCAQEAq+Eyilvjrwv8\r\n" \
"B/gZjdM2fXToFByASxDSl6/nRjKN5l6fcHJU84efgeafTdy4ZSmOvi6McZx6nK41\r\n" \
"YtAFAZORA5s0BKGxc3X5egWlOOYDooOvCrYfT60U1R5pdTOl3Pj5J5O+RIGIW1QL\r\n" \
"Q9OgUoeW1jyznqMZA2YrNAh1d72JspA/VMQvyYM9LrodFmrTYl2KMiEsQJtzz7Ya\r\n" \
"waHAjmvlLqIkP3t5DpLKixU1+mKwb/rUf4XaxPreyNCSXb/IcQep7L5yGWLVXwVV\r\n" \
"XA4v/FTD4XE305gmIZ5mB5ucPyer/WQQQusspe+OGxejJE1sNBZs3AMAsSzI0R7y\r\n" \
"bMbsUvEd6g==\r\n" \
"-----END CERTIFICATE-----\r\n" \
"-----BEGIN CERTIFICATE-----\r\n" \
"MIIEqjCCA5KgAwIBAgIQAnmsRYvBskWr+YBTzSybsTANBgkqhkiG9w0BAQsFADBh\r\n" \
"MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\r\n" \
"d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\r\n" \
"QTAeFw0xNzExMjcxMjQ2MTBaFw0yNzExMjcxMjQ2MTBaMG4xCzAJBgNVBAYTAlVT\r\n" \
"MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\r\n" \
"b20xLTArBgNVBAMTJEVuY3J5cHRpb24gRXZlcnl3aGVyZSBEViBUTFMgQ0EgLSBH\r\n" \
"MTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALPeP6wkab41dyQh6mKc\r\n" \
"oHqt3jRIxW5MDvf9QyiOR7VfFwK656es0UFiIb74N9pRntzF1UgYzDGu3ppZVMdo\r\n" \
"lbxhm6dWS9OK/lFehKNT0OYI9aqk6F+U7cA6jxSC+iDBPXwdF4rs3KRyp3aQn6pj\r\n" \
"pp1yr7IB6Y4zv72Ee/PlZ/6rK6InC6WpK0nPVOYR7n9iDuPe1E4IxUMBH/T33+3h\r\n" \
"yuH3dvfgiWUOUkjdpMbyxX+XNle5uEIiyBsi4IvbcTCh8ruifCIi5mDXkZrnMT8n\r\n" \
"wfYCV6v6kDdXkbgGRLKsR4pucbJtbKqIkUGxuZI2t7pfewKRc5nWecvDBZf3+p1M\r\n" \
"pA8CAwEAAaOCAU8wggFLMB0GA1UdDgQWBBRVdE+yck/1YLpQ0dfmUVyaAYca1zAf\r\n" \
"BgNVHSMEGDAWgBQD3lA1VtFMu2bwo+IbG8OXsj3RVTAOBgNVHQ8BAf8EBAMCAYYw\r\n" \
"HQYDVR0lBBYwFAYIKwYBBQUHAwEGCCsGAQUFBwMCMBIGA1UdEwEB/wQIMAYBAf8C\r\n" \
"AQAwNAYIKwYBBQUHAQEEKDAmMCQGCCsGAQUFBzABhhhodHRwOi8vb2NzcC5kaWdp\r\n" \
"Y2VydC5jb20wQgYDVR0fBDswOTA3oDWgM4YxaHR0cDovL2NybDMuZGlnaWNlcnQu\r\n" \
"Y29tL0RpZ2lDZXJ0R2xvYmFsUm9vdENBLmNybDBMBgNVHSAERTBDMDcGCWCGSAGG\r\n" \
"/WwBAjAqMCgGCCsGAQUFBwIBFhxodHRwczovL3d3dy5kaWdpY2VydC5jb20vQ1BT\r\n" \
"MAgGBmeBDAECATANBgkqhkiG9w0BAQsFAAOCAQEAK3Gp6/aGq7aBZsxf/oQ+TD/B\r\n" \
"SwW3AU4ETK+GQf2kFzYZkby5SFrHdPomunx2HBzViUchGoofGgg7gHW0W3MlQAXW\r\n" \
"M0r5LUvStcr82QDWYNPaUy4taCQmyaJ+VB+6wxHstSigOlSNF2a6vg4rgexixeiV\r\n" \
"4YSB03Yqp2t3TeZHM9ESfkus74nQyW7pRGezj+TC44xCagCQQOzzNmzEAP2SnCrJ\r\n" \
"sNE2DpRVMnL8J6xBRdjmOsC3N6cQuKuRXbzByVBjCqAA8t1L0I+9wXJerLPyErjy\r\n" \
"rMKWaBFLmfK/AHNF4ZihwPGOc7w6UHczBZXH5RFzJNnww+WnKuTPI0HfnVH8lg==\r\n" \
"-----END CERTIFICATE-----\r\n" 

#define MQTT_PRIKEY \
"-----BEGIN RSA PRIVATE KEY-----\r\n" \
"MIIEpQIBAAKCAQEAuH221NBjVKf56TqcNeDMxldarerckCuuOXa1fhjNENOCK1Bh\r\n" \
"UdHFyiUHawVnJQ7UvB31fgVHlHDejLfHDZpBbtkzOTeaixSFeZDCggpU1CDvdnzR\r\n" \
"TDhwcUFnyS+fPc3jSXD6EtNxIlDcyvQHhoIRekepZn2ipxzWXHArIr2BWvLyH1Cm\r\n" \
"Syup1O3VZJA35+ff4vX8l7uH5o7TIWWSVkKrGW3ONMMs2WSMgCSjamItU4VGA1q+\r\n" \
"sZRYRsTR0EnbooakufcnStVE75/27WF0Vt7D3MUy4xYuoWDgF79uLFcWywCATO+G\r\n" \
"y6wWA5gM/oyMans99slL4TzsRGYpJNY/ARezoQIDAQABAoIBAAGdqEJI6PXO2tN+\r\n" \
"Hewi4ehCz5qCWhgxKb97GK/YC8O/9n3moFOVIMRaeqwnSfX7IUXchU/rP+s+6dQV\r\n" \
"r/qghT4xTM+Nex8IuPqNmAaHjKKm/zHsefDO9b0oM36BmkVyTjFQegA8Ndt1Fdn5\r\n" \
"NS+qZ/jIM+Wf6BLKn94DZX1SZ5WiU0n+ungfkAQkIYv43cytpS3g4NRFi/tPtjL9\r\n" \
"NFBrhTi1eyFwGcxgsTLuCQse0fwTqxQgqGRcIxMcBD7XYqYAKRyW8Xxqlb4sp+CG\r\n" \
"cXeBLKhxAgbCY3bnBIVGadYWHlXKgDYuFkKSWYGdi9ZO77hnx2gXpqSQ2cE4znkv\r\n" \
"cZGutHkCgYEA2aO7dhrRloRa0cvPqRtA5J7FPkzgwu4/47VjVm/Sa72sfWTUwgdq\r\n" \
"TNB0K1OEzpNcxtNqfmXwt0tR/fZHannZ1XeuEuh/MbFbJPzLz6Sr/6siFb2Me9Ui\r\n" \
"b1P4kMgDMpk9E0pKuulBPYuPZ7fJqk1GJK9xPqAK+at0JA/P2Ei9M88CgYEA2QJD\r\n" \
"z3l7BEh7EdKMCFAFmHfmZJS4ws2hN1w4EFxBIdITczCbzZ4GCI0+ToriOqvMG2A5\r\n" \
"KYq79vLvGe+U9I4AKQPSaWiMalG2+KkVM2/nxF1+VXIQSdwVqp0BPYDkNIN0CbI7\r\n" \
"WIZFWO4y0CgWt5eiAUCg3eH6gGQRYBF+e6AlzY8CgYEA1HSpQxnbD3wUzEiRZxtb\r\n" \
"tdHn6nFR1/iMFdW0K2TtnI+EGsSbjO2AL4wuWz0koCPqpY0soEcTPVshpzELdj2N\r\n" \
"EH3MSFj+EZx4xRqiOwkdfm+ozmBDC398JgFq3KrjtDkzDemwGdYqAdKDI1SeYMke\r\n" \
"eCFsAx5omGFu9uUYmKIpULECgYEAjhQpiYEjxZ4e1YBgJmEeCMEXmuq0PPacgr5i\r\n" \
"BnQgQKBmE5jTqT+4+4heLtZHX9py+8l4I3atsfVEDlsJz0PZXwEdld3ZcRKnFQ6+\r\n" \
"zrqJp8r+l7riZzktBlNdKINm5era4i5LJJ5kcOEja52ToggpDWDe8eauN7lMlvVd\r\n" \
"NluX5IkCgYEApMRjVZOj9DuiDxXfU/Q7WHiSCqSiBEamiutFp+co+lPEtoJqwaDR\r\n" \
"kKEfd/luads8pTj6L1wl+ADfUwcXbu8DbLTA4tu5yAylY8XmEV+zMe+s9yMTrGl7\r\n" \
"STSQztSxHc6VqLvsCscsRuEfown/UC1PU5BqanK/VrLKetVi9iRDe8o=        \r\n" \
"-----END RSA PRIVATE KEY-----\r\n"

#define REGIST_KEY_CRT  \
"-----BEGIN CERTIFICATE-----\r\n" \
"MIIGBTCCBO2gAwIBAgIQAbrbQRTcz7dmsAdVppm8XDANBgkqhkiG9w0BAQsFADBu\r\n" \
"MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\r\n" \
"d3cuZGlnaWNlcnQuY29tMS0wKwYDVQQDEyRFbmNyeXB0aW9uIEV2ZXJ5d2hlcmUg\r\n" \
"RFYgVExTIENBIC0gRzEwHhcNMjEwNDA4MDAwMDAwWhcNMjIwNDA4MjM1OTU5WjAj\r\n" \
"MSEwHwYDVQQDExhpb3RodWItY2xpZW50MS5pbnNwb3MuY24wggEiMA0GCSqGSIb3\r\n" \
"DQEBAQUAA4IBDwAwggEKAoIBAQCwlH6UcJoUZdX1T3Nyq8m38Qv3Lsm4Wq0zDTnq\r\n" \
"RWNnSAoHvNaSv9XSObKkRqwFdfYX7HODPcn4hnE0WSHPfxj1o4gSyBtKu65PgVZu\r\n" \
"uqq4smqc56wSdxKvbOvxtEEyv8i8pF45aTgsRu/S1XHt3Q9MIeHLUg24iMYU3He1\r\n" \
"ibq3HKRh2bnEqlYkUDVzehXRoZnZuzne/jFZvh8LUs7dvcrlA7oclNM5bp8YKsBD\r\n" \
"qp6VP+M9PZOK4yQt9w/KVHJWM4o0tTJnKWlFXBeShmjuqmC/tP9HmaDxodDstErB\r\n" \
"gbD4SnfkzudAO/rCnrmKb7ie/a5XIdqesettELOPj0+g5di3AgMBAAGjggLoMIIC\r\n" \
"5DAfBgNVHSMEGDAWgBRVdE+yck/1YLpQ0dfmUVyaAYca1zAdBgNVHQ4EFgQUlAHD\r\n" \
"Qqeyk9MRARJC38fPo9x9LSkwIwYDVR0RBBwwGoIYaW90aHViLWNsaWVudDEuaW5z\r\n" \
"cG9zLmNuMA4GA1UdDwEB/wQEAwIFoDAdBgNVHSUEFjAUBggrBgEFBQcDAQYIKwYB\r\n" \
"BQUHAwIwPgYDVR0gBDcwNTAzBgZngQwBAgEwKTAnBggrBgEFBQcCARYbaHR0cDov\r\n" \
"L3d3dy5kaWdpY2VydC5jb20vQ1BTMIGABggrBgEFBQcBAQR0MHIwJAYIKwYBBQUH\r\n" \
"MAGGGGh0dHA6Ly9vY3NwLmRpZ2ljZXJ0LmNvbTBKBggrBgEFBQcwAoY+aHR0cDov\r\n" \
"L2NhY2VydHMuZGlnaWNlcnQuY29tL0VuY3J5cHRpb25FdmVyeXdoZXJlRFZUTFND\r\n" \
"QS1HMS5jcnQwCQYDVR0TBAIwADCCAX4GCisGAQQB1nkCBAIEggFuBIIBagFoAHUA\r\n" \
"RqVV63X6kSAwtaKJafTzfREsQXS+/Um4havy/HD+bUcAAAF4sOzeLwAABAMARjBE\r\n" \
"Ah9wtli4f622o7qOBg8VB5FFHChSUMoKaWK/ZhWIgVg2AiEAhU5FwFBSn/S/XdRv\r\n" \
"iHHGXnv4Jev8EGgIjOWcFO75m3sAdwAiRUUHWVUkVpY/oS/x922G4CMmY63AS39d\r\n" \
"xoNcbuIPAgAAAXiw7N48AAAEAwBIMEYCIQCsmZ6LfuLZpYwSgwUGNtT0fmUCDuM4\r\n" \
"ve5wbemJ9ebzMQIhANy/0XWRiQLvxbW4L7Dm8fa9o5bEfug3vAPvfybxwlgbAHYA\r\n" \
"QcjKsd8iRkoQxqE6CUKHXk4xixsD6+tLx2jwkGKWBvYAAAF4sOzd5AAABAMARzBF\r\n" \
"AiAcZVtk9uPcnwTwLsUivSPri3pMr/YS4hvfm9/C0YNqVgIhAJtzDLvYm5ofK0Et\r\n" \
"y9xhJ9tyItBLyE+MQbFHYLCTGLNpMA0GCSqGSIb3DQEBCwUAA4IBAQBhiRC9b91B\r\n" \
"ndIprtLbB5Cvo+R3i2aa9SET6Sx2mPPhdt22EpPOezBYdrrY+1sVaiXS9hsNna+/\r\n" \
"xSKlw+jnolonCtWLuBQpyc0l5hE1nJYXhYHKx3mO11AGVIoNrlD/FIjMjjYorDn7\r\n" \
"LThMBTFtEjNKDwhMOuNaGBK+aR2ePtj7NTL5JY+2s6uxU+V8efYPJjTEFWEUcPZQ\r\n" \
"4mHkvV7pGO17ZiW/ct7Sg5Zx/pHljmOQT9QdsEwqXldMagS7Gt5aI2IJ+uSopy/K\r\n" \
"U4DzV4C8IrX2e9/58o5nsUdqtZ9yR1PDvvi83sojJl6LpIlKwJHyWPYs7ScO9Y9b\r\n" \
"VzFMHztWIhGn\r\n" \
"-----END CERTIFICATE-----\r\n" \
"-----BEGIN CERTIFICATE-----\r\n" \
"MIIEqjCCA5KgAwIBAgIQAnmsRYvBskWr+YBTzSybsTANBgkqhkiG9w0BAQsFADBh\r\n" \
"MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\r\n" \
"d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\r\n" \
"QTAeFw0xNzExMjcxMjQ2MTBaFw0yNzExMjcxMjQ2MTBaMG4xCzAJBgNVBAYTAlVT\r\n" \
"MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\r\n" \
"b20xLTArBgNVBAMTJEVuY3J5cHRpb24gRXZlcnl3aGVyZSBEViBUTFMgQ0EgLSBH\r\n" \
"MTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALPeP6wkab41dyQh6mKc\r\n" \
"oHqt3jRIxW5MDvf9QyiOR7VfFwK656es0UFiIb74N9pRntzF1UgYzDGu3ppZVMdo\r\n" \
"lbxhm6dWS9OK/lFehKNT0OYI9aqk6F+U7cA6jxSC+iDBPXwdF4rs3KRyp3aQn6pj\r\n" \
"pp1yr7IB6Y4zv72Ee/PlZ/6rK6InC6WpK0nPVOYR7n9iDuPe1E4IxUMBH/T33+3h\r\n" \
"yuH3dvfgiWUOUkjdpMbyxX+XNle5uEIiyBsi4IvbcTCh8ruifCIi5mDXkZrnMT8n\r\n" \
"wfYCV6v6kDdXkbgGRLKsR4pucbJtbKqIkUGxuZI2t7pfewKRc5nWecvDBZf3+p1M\r\n" \
"pA8CAwEAAaOCAU8wggFLMB0GA1UdDgQWBBRVdE+yck/1YLpQ0dfmUVyaAYca1zAf\r\n" \
"BgNVHSMEGDAWgBQD3lA1VtFMu2bwo+IbG8OXsj3RVTAOBgNVHQ8BAf8EBAMCAYYw\r\n" \
"HQYDVR0lBBYwFAYIKwYBBQUHAwEGCCsGAQUFBwMCMBIGA1UdEwEB/wQIMAYBAf8C\r\n" \
"AQAwNAYIKwYBBQUHAQEEKDAmMCQGCCsGAQUFBzABhhhodHRwOi8vb2NzcC5kaWdp\r\n" \
"Y2VydC5jb20wQgYDVR0fBDswOTA3oDWgM4YxaHR0cDovL2NybDMuZGlnaWNlcnQu\r\n" \
"Y29tL0RpZ2lDZXJ0R2xvYmFsUm9vdENBLmNybDBMBgNVHSAERTBDMDcGCWCGSAGG\r\n" \
"/WwBAjAqMCgGCCsGAQUFBwIBFhxodHRwczovL3d3dy5kaWdpY2VydC5jb20vQ1BT\r\n" \
"MAgGBmeBDAECATANBgkqhkiG9w0BAQsFAAOCAQEAK3Gp6/aGq7aBZsxf/oQ+TD/B\r\n" \
"SwW3AU4ETK+GQf2kFzYZkby5SFrHdPomunx2HBzViUchGoofGgg7gHW0W3MlQAXW\r\n" \
"M0r5LUvStcr82QDWYNPaUy4taCQmyaJ+VB+6wxHstSigOlSNF2a6vg4rgexixeiV\r\n" \
"4YSB03Yqp2t3TeZHM9ESfkus74nQyW7pRGezj+TC44xCagCQQOzzNmzEAP2SnCrJ\r\n" \
"sNE2DpRVMnL8J6xBRdjmOsC3N6cQuKuRXbzByVBjCqAA8t1L0I+9wXJerLPyErjy\r\n" \
"rMKWaBFLmfK/AHNF4ZihwPGOc7w6UHczBZXH5RFzJNnww+WnKuTPI0HfnVH8lg==\r\n" \
"-----END CERTIFICATE-----\r\n" 


#define REGISTER_PRIKEY \
"-----BEGIN RSA PRIVATE KEY-----\r\n" \
"MIIEpAIBAAKCAQEAsJR+lHCaFGXV9U9zcqvJt/EL9y7JuFqtMw056kVjZ0gKB7zW\r\n" \
"kr/V0jmypEasBXX2F+xzgz3J+IZxNFkhz38Y9aOIEsgbSruuT4FWbrqquLJqnOes\r\n" \
"EncSr2zr8bRBMr/IvKReOWk4LEbv0tVx7d0PTCHhy1INuIjGFNx3tYm6txykYdm5\r\n" \
"xKpWJFA1c3oV0aGZ2bs53v4xWb4fC1LO3b3K5QO6HJTTOW6fGCrAQ6qelT/jPT2T\r\n" \
"iuMkLfcPylRyVjOKNLUyZylpRVwXkoZo7qpgv7T/R5mg8aHQ7LRKwYGw+Ep35M7n\r\n" \
"QDv6wp65im+4nv2uVyHanrHrbRCzj49PoOXYtwIDAQABAoIBABCUwRYbRLypm1+/\r\n" \
"qC2aaAS3w9HgONzw1ta2ZTvUbDMklpyHZPa+wMtO07GNWVXh78vrTiWCxdZqeXLa\r\n" \
"88vSOXvHcV2iIL3xxbp9gKpBKQrS5iP5tTS+rah/Z+B4dq7XR3g/TwHBGZ8+wgJg\r\n" \
"PKSDX45cmCFJgTRU05Td5vfLCRdslwhfneIjPiTLA47WSK5dNEDzLt4Zfx+3Ci8u\r\n" \
"6Q1QHB11NoaQeSrZSrNwWhehCYd4ZrHl+RbwtD3I+uOokXIitYF3rY3AqoCO3/GY\r\n" \
"PElsJtsp8oSUYlxVA+dnjGwCviKcplgpRfJiXG8H1jASqC+JCt9sGhDYfOp2BL4+\r\n" \
"bTRs7bECgYEA52Aqjs0NwCsw7MHGa9hWcolunMQy6d+bjFligycFj2zMZ72XMLHY\r\n" \
"sZPBhUxVWAAnzLdbpoj3mtIWGwdXoRzv8XXKnAbDc0wBZ77ddg/XAELmFFJtoouo\r\n" \
"46IaIq9B5kKa18lsUPWpFncqsWAPQSdQ0uNQEFKSOFHGD4rjxH0vUsMCgYEAw19r\r\n" \
"2ZwhW6ZgFJ5SwihOH3yVgK5J8FGHFI7Imb597zHxGnioSPQjzIpbha29OZUY1HON\r\n" \
"GyT0eQghfViZmXOpnLtfcCETVa8VnJarHeXhF80Pk2lcoBr0LMED/dM4TEaVi3Ct\r\n" \
"yyLgJgvg+HHkUxocE2zfn1Pr4JOOkDTLA+hN2v0CgYEAveZa55t65rdMT3V3Qnpr\r\n" \
"8VPE+lu5oeRKCXYveY9ka+QTHC4qUegV/yFTWvFctn5iLA5t5ZBTELnPNN3g2TYj\r\n" \
"5R0VbtvKPO4r1Rd9MnIrt2Yw6osAusE/m50nMEKBptgRuHv0L7qAzhR+cNhzjLkV\r\n" \
"CBFTXCI5KopLMeR/rnxEg/sCgYEAsy0n9ALEdRE4vLAWpgng2++eDM3K8aNThv8L\r\n" \
"6jRRurTKscRGUeHVkxiwpwCXsd+TTgRLPMG/k+BpFlPTSnBQ9XTCKkurN8HCyEFF\r\n" \
"3b2dmf5ucnsGf684kHLT/k1gLFPvglpsiQgLGdl7EV8iTzR62zXWQIDuqp+o94fz\r\n" \
"SDocbOECgYB9zS1DIMAxQQz+JfInmUTvVESGIaml8WCFa5lNVX07gnf2Z32tWnef\r\n" \
"trzznuFb03/e+7dmWDsQFi0I5tezG+xFC5Ofsd14HyD+cofNvQ8fA5eVUIYcC4eO\r\n" \
"fHStb0v8B6dXHnejonYch8FCRN9JqybHB/3NGGDtpaN98E5gKFQNcw==\r\n" \
"-----END RSA PRIVATE KEY-----\r\n"


#define SN_ID "1000YRV6020F21070000000001" //��Ӧ��Կbd40b6a780820a51

static const char *mqtt_keycer_buf = MQTT_KEY_CRT;
static const char *mqtt_prikey_buf = MQTT_PRIKEY;
static const char *regist_keycer_buf = REGIST_KEY_CRT;
static const char *server_cacer_buf = SERVER_CA_CRT;
static const char *server_prikey_buf = REGISTER_PRIKEY;

#define PRODUCT_KEY "YY1vtz6SlUma1n0X"
#define PRODUCT_SECR "r13uHIO62XeMPs55"  
#define TIME_STAMP "1540800420"
#define DEV_SECRET "bd40b6a780820a51"

static char dec2hex[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
char tempsignbuf[1024];
char subtopic[256];

int register_pack_post(uint8 *deviceName,uint8 *timestamp,uint8 *productkey,uint8 *proSret,char *signmd,uint8 *outpack)
{
	mbedtls_md5_context md5_info;
	char signstring[33] = { 0 };
	char  sign[17] = { 0 };
	cJSON *root = NULL;
	int k;

	memset(tempsignbuf,0x00,sizeof(tempsignbuf));
	
	strcat((char*)tempsignbuf,"deviceName=");
	strcat((char*)tempsignbuf,(char*)deviceName);
	strcat((char*)tempsignbuf,"&");
	
	strcat((char*)tempsignbuf,"productKey=");
	strcat((char*)tempsignbuf,(char*)productkey);
	strcat((char*)tempsignbuf,"&");

	strcat((char*)tempsignbuf,"timestamp=");
	strcat((char*)tempsignbuf,(char*)timestamp);
	strcat((char*)tempsignbuf,"&");

	strcat((char*)tempsignbuf, "key=");
	strcat((char*)tempsignbuf, (char*)proSret);

	mbedtls_md5_init(&md5_info);
	mbedtls_md5_starts(&md5_info);
	mbedtls_md5_update(&md5_info,(unsigned char*) tempsignbuf, strlen((char *)tempsignbuf));
	mbedtls_md5_finish(&md5_info, sign);
	mbedtls_md5_free(&md5_info);


	for (k = 0;k < 16;k++)
	{
		signstring[k*2] = dec2hex[(sign[k]&0xF0)>>4];
		signstring[k * 2+1] = dec2hex[sign[k] & 0x0F];
	}

	for (k = 0;k < 32;k++) {
		if (signstring[k] >= 'a' && signstring[k] <= 'z') {
			signstring[k] -= ('a' - 'A');
		}
	}

	root = cJSON_CreateObject();
	if (!root)
	{
		return -1;
	}

	cJSON_AddStringToObject(root, "deviceName", deviceName);
	cJSON_AddStringToObject(root, "sign", signstring);
	cJSON_AddStringToObject(root, "timestamp", timestamp);
	cJSON_AddStringToObject(root, "productKey", productkey);
	cJSON_AddStringToObject(root, "signMethod", signmd);

	char *sout = cJSON_PrintUnformatted(root);

	strcpy(outpack, sout);
	cJSON_Delete(root);
	LocalFree(sout);
	root = NULL;
	
	return 0;
}

int app_registe_device(void)
{
	char pack_buf[1024];
	int ret;
	char recv_buff[2048] = {0};
	int recv_len = 0;

	
	memset(pack_buf,0,sizeof(pack_buf));
	ret = register_pack_post(SN_ID,TIME_STAMP,PRODUCT_KEY,PRODUCT_SECR,"md5",pack_buf);

	MercuryTrace("register_pack, pack_buf:%s\n", pack_buf);

	qyy_setx509cer(server_cacer_buf,strlen(server_cacer_buf)+1);
	qyy_setx509_own_cer(regist_keycer_buf, strlen(regist_keycer_buf)+1);
	qyy_setx509_prikey(server_prikey_buf, strlen(server_prikey_buf)+1);

	qyy_http_post(SERVER_REGISTER_URL, pack_buf, "application/json", recv_buff, 2048,&recv_len);
	if (recv_len)
		MercuryTrace("regist result:%s\n", recv_buff);

	return 0;
}

void app_mqtt_msgcb(MessageData *data)
{
	MercuryTrace("recv message topic:%s, payload:%s\n", data->topicName->lenstring.data, data->message->payload);
	// // sdk_voiceplay(1,"�յ���Ϣ");
	// // sdk_voiceplay(1,"Message Received");
}

int app_mqtt_sub(int handle)
{
	int ret;
	
	memset(subtopic,0,sizeof(subtopic));
	memcpy(subtopic + strlen((const char*)subtopic),"/v2/user/down/", strlen("/v2/user/down/"));
	memcpy(subtopic + strlen((const char*)subtopic), PRODUCT_KEY, strlen(PRODUCT_KEY));
	memcpy(subtopic + strlen((const char*)subtopic), "/",1);
	memcpy(subtopic + strlen((const char*)subtopic), SN_ID, strlen(SN_ID));
	
	MercuryTrace("subtopic:%s",subtopic);
	
	ret = sdk_MQTT_subcribe(handle, subtopic, 1, app_mqtt_msgcb);

	MercuryTrace("subscribe result:%d\n", ret);
	return ret;
}

void app_mqtt_yield_thread( ULONG argc, LPVOID lpThreadParameter )
{
	unsigned long ret = 0 ;
		 
	while (1)
	{
		sdk_MQTT_yield(argc,1);
		// sdk_MQTT_yield(argc,500);
		// here can handle mqtt connection lost
	}
	sdk_MQTT_close(argc);
	
	ExitThread(NULL);
}

int app_connect_mqtt(void)
{
	int ret;
	char  ipaddress[5] ={0};

	int mqtthandle = 0;

	// qyy_setx509cer(server_cacer_buf,strlen(server_cacer_buf)+1);
	// qyy_setx509_own_cer(mqtt_keycer_buf, strlen(mqtt_keycer_buf)+1);
	// qyy_setx509_prikey(mqtt_prikey_buf, strlen(mqtt_prikey_buf)+1);

	ret = qyy_hosttoip((char *)MQTT_SERVER_URL,(char*)ipaddress);
	
	// ret = qyy_MQTT_connect((char*)ipaddress,8883, 1,
	// 							60,SN_ID,
	// 							SN_ID,DEV_SECRET,
	// 							90,NULL,
	// 							&mqtthandle, 0, NULL,NULL,60,1);
	
	ret = qyy_MQTT_connect((char*)ipaddress,8883,
								1,60,
								"Mos-91200511940236",
								"Mos-91200511940236",
								"Mq@63204911500219#Mos",
								90, NULL,
								&mqtthandle, 0, NULL, NULL, 60, 1);

	MercuryTrace("Lib_MqttConnect handle handle:%d",mqtthandle);
	
	if (mqtthandle)
	{
		app_mqtt_sub(mqtthandle);
		CreateThread(NULL, 8*1024, app_mqtt_yield_thread, mqtthandle, NULL, STACK_SIZE_RESERVATION, NULL);
	}
}


