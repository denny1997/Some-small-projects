namespace Miao_studio.Models
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel.DataAnnotations;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Data.Entity.Spatial;

    [Table("input")]
    public partial class input
    {
        [StringLength(50)]
        public string Id { get; set; }

        [Required]
        [StringLength(50)]
        public string name { get; set; }

        [Required]
        [StringLength(50)]
        public string type { get; set; }

        public int number { get; set; }

        public decimal price { get; set; }

        [Column(TypeName = "date")]
        public DateTime time { get; set; }

        [Column("operator")]
        [Required]
        [StringLength(50)]
        public string _operator { get; set; }

        [Required]
        [StringLength(50)]
        public string provider { get; set; }

        [Required]
        [StringLength(50)]
        public string unit { get; set; }
    }
}
