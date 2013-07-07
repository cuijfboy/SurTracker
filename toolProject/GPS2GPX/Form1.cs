using System;
using System.IO;

using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Xml;

namespace GPS2GPX
{
    public partial class Form_main : Form
    {
        private String rawFileName;
        private String gpxFileName;
        private XmlDocument doc;
        private XmlNode root;
        private XmlElement trkseg;

        private String[] rawMsg;
        private String lat2;
        private String lon2;
        private String time2;
        //private bool gps_msg_ready;

        public Form_main()
        {
            InitializeComponent();
            rawMsg = new String[12];
        }

        private bool transform(String fileName)
        {
            bool answer = false;
            StreamReader inStr = null;
            String textLine = null;

            FileInfo textFile = new FileInfo(fileName);
            inStr = textFile.OpenText();
            textLine = inStr.ReadLine();
            this.newGpxFile();
            textLine = inStr.ReadLine();
            while (textLine != null && !(textLine.Trim().Equals("")))
            {
                this.analyseRawLine(textLine);
                this.convertDataFomat(rawMsg[3], rawMsg[5], rawMsg[1], rawMsg[0]);
                this.addTrkpt(this.lat2, this.lon2, this.time2);
                textLine = inStr.ReadLine();
            }
            inStr.Close();
            gpxFileName = rawFileName.Substring(0, rawFileName.LastIndexOf('\\')) + rawFileName.Substring(rawFileName.LastIndexOf('\\'), rawFileName.LastIndexOf('.') - rawFileName.LastIndexOf('\\')) + ".gpx";
            //MessageBox.Show(gpxFileName);
            this.saveGpxFile(gpxFileName);
            MessageBox.Show("转换完毕！");
            return answer;
        }
        private void analyseRawLine(String rawLine)
        {
            int temp = 0;
            int tokenPos = rawLine.IndexOf('|');
            rawLine = rawLine.Substring(tokenPos + 1);
            tokenPos = rawLine.IndexOf('|');
            while (tokenPos > 0)
            {
                //MessageBox.Show(rawLine.Substring(0, tokenPos));
                this.rawMsg[temp++] = rawLine.Substring(0, tokenPos);
                rawLine = rawLine.Substring(tokenPos + 1);
                tokenPos = rawLine.IndexOf('|');
            }
        }

        private void newGpxFile()
        {
            doc = new XmlDocument();
            root = doc.CreateElement("gpx");
            doc.AppendChild(root);

            XmlElement trk = doc.CreateElement("trk");
            root.AppendChild(trk);

            XmlElement name = doc.CreateElement("name");
            name.InnerText = rawFileName.Substring(rawFileName.LastIndexOf('\\') + 1, rawFileName.LastIndexOf('.') - rawFileName.LastIndexOf('\\') - 1);
            trk.AppendChild(name);

            trkseg = doc.CreateElement("trkseg");
            trk.AppendChild(trkseg);
        }

        private void convertDataFomat(String latStr, String lonStr, String timeStr, String dateStr)
        {
            double lat = Convert.ToDouble(latStr.Substring(0, 2)) + Convert.ToDouble(latStr.Substring(2)) / 60;
            this.lat2 = Convert.ToString(lat);
            double lon = Convert.ToDouble(lonStr.Substring(0, 3)) + Convert.ToDouble(lonStr.Substring(3)) / 60;
            this.lon2 = Convert.ToString(lon);
            this.time2 = "20" + dateStr.Substring(4, 2) + "-" + dateStr.Substring(2, 2) + "-" + dateStr.Substring(0, 2) + "T" + timeStr.Substring(0, 2) + ":" + timeStr.Substring(2, 2) + ":" + timeStr.Substring(4, 2) + "Z";
        }

        private void addTrkpt(String latStr, String lonStr, String timeStr)
        {
            XmlElement trkpt = doc.CreateElement("trkpt");
            trkpt.SetAttribute("lat", latStr);
            trkpt.SetAttribute("lon", lonStr);

            XmlElement time = doc.CreateElement("time");
            time.InnerText = timeStr;
            trkpt.AppendChild(time);

            trkseg.AppendChild(trkpt);
        }

        private void saveGpxFile(String filename)
        {
            XmlWriter xw = XmlWriter.Create(filename);
            doc.WriteTo(xw);
            xw.Flush();
            xw.Close();
        }
        private void openFile()
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.Personal);
            openFileDialog.Filter = "RAW数据文件(*.raw)|*.raw|日志文件(*.log)|*.log|文本文件(*.txt)|*.txt|所有文件(*.*)|*.*";

            if (openFileDialog.ShowDialog(this) == DialogResult.OK)
            {
                this.rawFileName = openFileDialog.FileName;
                MessageBox.Show(rawFileName);
            }
        }

        private void open_ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            openFile();
        }

        private void button_open_Click(object sender, EventArgs e)
        {
            openFile();
        }

        private void button_trans_Click(object sender, EventArgs e)
        {
            //transform(rawFileName);
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.Personal);
            openFileDialog.Filter = "RAW数据文件(*.raw)|*.raw|日志文件(*.log)|*.log|文本文件(*.txt)|*.txt|所有文件(*.*)|*.*";

            if (openFileDialog.ShowDialog(this) == DialogResult.OK)
            {
                this.rawFileName = openFileDialog.FileName;
                transform(rawFileName);
            }
        }
    }
}
