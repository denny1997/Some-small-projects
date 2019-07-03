using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace Miao_studio.Models
{
    public class StockExcel
    {
        public string name { get; set; }
        public string type { get; set; }
        public string unit { get; set; }
        public int former_stock { get; set; }
        public decimal former_total { get; set; }
        public int latter_stock { get; set; }
        public decimal latter_total { get; set; }
        public int delta_inputstock { get; set; }
        public decimal delta_outputtotal { get; set; }
        public int delta_outputstock { get; set; }
        public decimal delta_inputtotal { get; set; }
        public StockExcel(string name, string type, string unit, int former_stock, decimal former_total, int delta_inputstock, decimal delta_inputtotal, int delta_outputstock, decimal delta_outputtotal, int latter_stock, decimal latter_total)
        {
            this.name = name;
            this.type = type;
            this.unit = unit;
            this.former_stock = former_stock;
            this.former_total = former_total;
            this.latter_stock = latter_stock;
            this.latter_total = latter_total;
            this.delta_inputstock = delta_inputstock;
            this.delta_outputtotal = delta_outputtotal;
            this.delta_inputtotal = delta_inputtotal;
            this.delta_outputstock = delta_outputstock;
        }
    }
}