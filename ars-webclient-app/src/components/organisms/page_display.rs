use yew::prelude::*;

use super::side_bar::SideBar;
use super::nav_bar::NavBar;
use super::character_info::CharacterInfo;
use super::details_display::DetailsDisplay;

pub struct PageDisplay {
    sidebar_open: bool,
}

#[derive(Clone, PartialEq, Properties)]
pub struct Props {
}

pub enum Msg {
    ToggleSidebar,
}

impl Component for PageDisplay {
    type Message = Msg;
    type Properties = Props;

    fn create(_ctx: &Context<Self>) -> Self {
        Self {
            sidebar_open: false,
        }
    }

    fn update(&mut self, ctx: &Context<Self>, msg: Self::Message) -> bool {
        match msg {
            Msg::ToggleSidebar => {
                // Keep track of the name of the child that was clicked
                self.sidebar_open = !self.sidebar_open;
                true
            }
        }
    }

    fn view(&self, ctx: &Context<Self>) -> Html {

        let sidebar_menu_click = ctx.link().callback(|_| Msg::ToggleSidebar);

        html! {
            <>
            <NavBar menu_clicked={sidebar_menu_click}/>
            if self.sidebar_open {
                <SideBar/>
                <div class="display-grid-2" style="margin-left:20%">
                    <CharacterInfo/>
                    <DetailsDisplay/>
                </div>
            } else {
                <div class="display-grid-2">
                <CharacterInfo/>
                <DetailsDisplay/>
            </div>
            }
            </>
        }
    }
}