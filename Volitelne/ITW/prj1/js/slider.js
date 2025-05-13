/**
 * Slider.
 * 
 * @author Jiri Hynek
 */
window.addEventListener("load", () => {

    const setActiveSliderItem = function(sliderItems, index) {
        // remove active class from all items
        for(let i = 0; i < sliderItems.length; i++) {
            sliderItems[i].classList.remove('active');
        }
        if(index >=0 && index < sliderItems.length) {
            // add active class to the selected item
            sliderItems[index].classList.add('active');
        }
    }

    const rotateSliderItems = function(sliderItems, activeIndex, time) {
        setActiveSliderItem(sliderItems, activeIndex);
        setTimeout(() => {
            //console.log("timeout", activeIndex);
            activeIndex = ++activeIndex%sliderItems.length;
            rotateSliderItems(sliderItems, activeIndex, time);
        }, time*1000);
    }

    const setUpSlider = function(slider) {
        const sliderItems = slider.getElementsByClassName('slider-item');
        //console.log(sliderItems);

        let timeValue = window.getComputedStyle(slider).getPropertyValue("--slider-time");
        let time = timeValue && parseFloat(timeValue);
        //console.log(time);
        if(time && time > 0) {
            rotateSliderItems(sliderItems, 0, time);
        }
    }

    // setup all sliders in the document
    const sliders = document.getElementsByClassName('slider');
    for(let i = 0; i < sliders.length; i++) {
        setUpSlider(sliders[i]);
    }
});