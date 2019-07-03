namespace Miao_studio.Models
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel.DataAnnotations;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Data.Entity.Spatial;

    public partial class out_put
    {
        [StringLength(50)]
        public string Id { get; set; }

        [Required]
        [StringLength(50)]
        public string name { get; set; }

        [Required]
        [StringLength(50)]
        public string type { get; set; }

        [Required]
        [StringLength(200)]
        public string detail { get; set; }

        [Column(TypeName = "date")]
        public DateTime time { get; set; }

        [Column("operator")]
        [Required]
        [StringLength(50)]
        public string _operator { get; set; }

        [Required]
        [StringLength(50)]
        public string project { get; set; }

        public decimal total { get; set; }

        public int number { get; set; }

        public string unit { get; set; }
    }
}
