/* script.js */
function onload_func()
{
    var date = new Date();
    document.getElementById('current-date').valueAsDate = new Date(date.getTime() - (date.getTimezoneOffset() * 60000)); // https://stackoverflow.com/questions/13646446/javascript-date-tojson-produces-a-date-which-has-wrong-hours-and-minutes
    document.getElementById('start-date').valueAsDate = new Date("2022-07-01");
    document.getElementById('end-date').valueAsDate = new Date("2030-01-01");
    document.getElementById('holiday-per-year').value = 9;
    document.getElementById('pto-per-year').value = 22;
    document.getElementById('sick-per-year').value = 18;

    calc();
}

function days_delta(d1, d2)
{
    return Math.round(Math.max(0, (d2 - d1) / (1000 * 60 * 60 * 24)));
}

function total_days()
{
    var d1 = new Date(document.getElementById("start-date").value);
    var d2 = new Date(document.getElementById("end-date").value);
    return days_delta(d1, d2);
}

function days_passed()
{
    var d1 = new Date(document.getElementById("start-date").value);
    var d2 = new Date(document.getElementById("current-date").value);
    return days_delta(d1, d2);
}

function days_remaining()
{
    var d1 = new Date(document.getElementById("current-date").value);
    var d2 = new Date(document.getElementById("end-date").value);
    return days_delta(d1, d2);
}

function weekend_days()
{
    var d1 = new Date(document.getElementById("current-date").value);
    var d2 = new Date(document.getElementById("end-date").value);
    if(d1 >= d2)
        return 0;
    var result = 0;
    while((d1 < d2) && (d1.getDay() != d2.getDay()))
    {
        if(d1.getDay() === 5 || d1.getDay() === 6) // Hebrew version
            result = result + 1;
        d1.setDate(d1.getDate() + 1);
    }
    return Math.round(result + 2 * (days_delta(d1, d2) / 7));
}

// returns estimated holidays that aren't weekends
function get_holiday()
{
    var per_day = parseInt(document.getElementById("holiday-per-year").value) / 365.25;
    return Math.floor(((days_remaining() - weekend_days()) * per_day));
}

function get_non_weekend_or_holiday_per_year()
{
    return (365.25 - parseInt(document.getElementById("holiday-per-year").value)) * 5.0 / 7.0;
}

function get_pto()
{
    var per_day = parseInt(document.getElementById("pto-per-year").value) / get_non_weekend_or_holiday_per_year();
    return Math.floor(((days_remaining() - weekend_days() - get_holiday()) * per_day));
}

function get_sick()
{
    var per_day = parseInt(document.getElementById("sick-per-year").value) / get_non_weekend_or_holiday_per_year();
    return Math.floor(((days_remaining() - weekend_days() - get_holiday()) * per_day));
}

function calc() {
    const tot = total_days();
    const daysPassed = days_passed()
    const weekenddays = weekend_days();
    const ptoDays = get_pto();
    const holidaysDays = get_holiday();
    const sickDays = get_sick();
    const workDays = Math.max(0, tot - (daysPassed + weekenddays + ptoDays + holidaysDays + sickDays));
    const daysPassedPercentage = (daysPassed / tot) * 100;
    const weekendsPercentage = (weekenddays / tot) * 100;
    const ptoPercentage = (ptoDays / tot) * 100;
    const holidaysPercentage = (holidaysDays / tot) * 100;
    const sickPercentage = (sickDays / tot) * 100;
    const workDaysPercentage = (workDays / tot) * 100;


    document.getElementById('days-passed-segment').style.width = `${daysPassedPercentage}%`;
    document.getElementById('weekends-and-holidays-segment').style.width = `${weekendsPercentage + holidaysPercentage}%`;
    document.getElementById('pto-segment').style.width = `${ptoPercentage}%`;
    document.getElementById('sick-segment').style.width = `${sickPercentage}%`;
    document.getElementById('work-segment').style.width = `${workDaysPercentage}%`;
    
    document.getElementById('percentage-days-passed').textContent = `${Math.round(daysPassedPercentage)}%`;
    document.getElementById('percentage-weekends-and-holidays').textContent = `${Math.round(weekendsPercentage + holidaysPercentage)}%`;
    document.getElementById('percentage-pto').textContent = `${Math.round(ptoPercentage)}%`;
    document.getElementById('percentage-sick').textContent = `${Math.round(sickPercentage)}%`;
    document.getElementById('percentage-work').textContent = `${Math.round(workDaysPercentage)}%`;

    document.getElementById('days-passed').textContent = daysPassed;
    document.getElementById('weekends-and-holidays').textContent = weekenddays + holidaysDays;
    document.getElementById('pto').textContent = ptoDays;
    document.getElementById('sick').textContent = sickDays;
    document.getElementById('work').textContent = workDays;

    document.getElementById('ttt').innerHTML = "Time till target: " + days_remaining() + " days";
}

document.getElementById('night-mode-toggle').addEventListener('click', function() {
    document.body.classList.toggle('night-mode');
});
