using Microsoft.Owin;
using Owin;

[assembly: OwinStartupAttribute(typeof(Miao_studio.Startup))]
namespace Miao_studio
{
    public partial class Startup
    {
        public void Configuration(IAppBuilder app)
        {
            ConfigureAuth(app);
        }
    }
}
