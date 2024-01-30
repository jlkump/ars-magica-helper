use crate::components::atoms:: {
    ars_button::ArsButton,
};

use crate::components::molecules::page_view_select::PageViewSelect;

use stylist::yew::styled_component;
use yew::prelude::*;


#[derive(Properties, Clone, PartialEq)]
pub struct Props {
}

#[styled_component(DetailsDisplay)]
pub fn details_display(_props: &Props) -> Html {
    html! {
        <div class="prevent-select block padding-small content">
            <PageViewSelect />
            <hr/>
            // <div class="grid-r-1 widget">
            //     <h3>{"View 1"}</h3>
            // </div>
            // <div class="grid-r-2 widget">
            //     <h3>{"Select View"}</h3>
            //     <div class="details-grid">                    
            //         <ArsButton label="Season Tracker" class="grid-c-1-3 grid-r-1"/>
            //         <ArsButton label="Personal Notes" class="grid-c-1-2 grid-r-2"/>
            //         <ArsButton label="Inventory" class="grid-c-1-2 grid-r-3"/>
            //         <ArsButton label="Virtues and Flaws" class="grid-c-2-3 grid-r-2"/>
            //         <ArsButton label="Twilight Scars" class="grid-c-2-3 grid-r-3"/>

            //         <h5 class="underlined center grid-c-3-4 grid-r-1">{"Magus Specific"}</h5>
            //         <ArsButton label="Spell Sheet" class="grid-c-3-4 grid-r-2"/>
            //         <ArsButton label="Laboratory" class="grid-c-3-4 grid-r-3"/>
            //         <h5 class="underlined center grid-c-4-5 grid-r-1">{"Shared"}</h5>
            //         <ArsButton label="Shared Maps" class="grid-c-4-5 grid-r-2"/>
            //         <ArsButton label="Shared Notes" class="grid-c-4-5 grid-r-3"/>
            //     </div>
            // </div>
        </div>
    }
}