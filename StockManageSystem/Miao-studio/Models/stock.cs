namespace Miao_studio.Models
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel.DataAnnotations;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Data.Entity.Spatial;

    [Table("stock")]
    public partial class stock
    {
        [Required]
        [StringLength(50)]
        public string name { get; set; }

        [Required]
        [StringLength(50)]
        public string type { get; set; }

        [Column("stock")]
        public int stock1 { get; set; }

        public decimal total { get; set; }

        public int id { get; set; }

        public string unit { get; set; }
    }
}
