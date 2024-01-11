use stylist::yew::styled_component;
use yew::prelude::*;

#[derive(Properties, Clone, PartialEq)]
pub struct Props {
}

#[styled_component(NavBar)]
pub fn nav_bar(_props: &Props) -> Html {
    html! {
        <span class={"navbar"}>
            <ul>
                <li class="left-align icon"><div class="center-row">
                    <img height="60px" width="100%" src="https://www.atlas-games.com/atlas-cms/resources/styles/arm-icon-w300h300.png"/>
                </div></li>
                <li>
                    <h1 class="prevent-select">{"Ars Magica Helper"}</h1>
                </li>
            </ul>
        </span>
    }
}