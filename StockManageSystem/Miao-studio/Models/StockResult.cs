using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace Miao_studio.Models
{
    public class StockResult
    {
        public string name { get; set; }
        public string type { get; set; }
        public string unit { get; set; }
        public int stock { get; set; }
        public decimal total { get; set; }
        public int dstock { get; set; }
        public decimal dtotal { get; set; }

        public StockResult(string name, string type, string unit, int stock, decimal total, int dstock, decimal dtotal)
        {
            this.name = name;
            this.type = type;
            this.unit = unit;
            this.stock = stock;
            this.total = total;
            this.dstock = dstock;
            this.dtotal = dtotal;
        }
    }
}